# switch dispatch method
# syntax
import sys, re

BASE_NAME = 'elgamal_impl::ElgamalT<elgamal_disp::local::FpType%d>::%s'
TYPE_NUM_TBL = [0, 3, 4, 6, 9]

# @dispatch(inClassName, code);
# inClassName : replace @inClassName in code with 'BASE_NAME % type + inClassName'

RE_BASE_FEATURE_NOSTR = re.compile('@baseFeatureNoStr\(([^,]*),[ ]*([^)]*)\)')
RE_BASE_FEATURE = re.compile('@baseFeature\(([^,]*),[ ]*([^)]*)\)')
RE_CLASSNAME = re.compile('@className\(([^)]*)\)')
RE_DISPATCH = re.compile('([ \t]*)@dispatch\((.*)\)')
RE_SET_BASENAME = re.compile('@setBaseName\(([^)]*)\)')

# stage 1. replace @baseFeature(<baseName>, <className>) with BASE_FEATURE
# stage 2. replace @baseName, @className with BASE_NAME % (type, <className>)

BASE_FEATURE_NOSTR = """	@baseName()
		: fpType_(System::getFpType())
		, self_(0)
	{
		@dispatch(self_ = (void*)new @className(@inClassName)();)
	}
	@baseName(const @baseName& rhs)
		: fpType_(rhs.fpType_)
		, self_(0)
	{
		@dispatch(self_ = (void*)new @className(@inClassName)(*(const @className(@inClassName)*)rhs.self_);)
	}
	~@baseName()
	{
		@dispatch(delete (@className(@inClassName)*)self_;)
	}
	@baseName& operator=(const @baseName& rhs)
	{
		@baseName tmp(rhs);
		swap(tmp);
		return *this;
	}
	void swap(@baseName& rhs) throw()
	{
		std::swap(fpType_, rhs.fpType_);
		std::swap(self_, rhs.self_);
	}"""

BASE_FEATURE = BASE_FEATURE_NOSTR + """
	std::string toStr() const throw(std::exception)
	{
		@dispatch(return ((const @className(@inClassName)*)self_)->toStr();)
	}
	void fromStr(const std::string& str) throw(std::exception)
	{
		@dispatch(((@className(@inClassName)*)self_)->fromStr(str);)
	}
	friend inline std::ostream& operator<<(std::ostream& os, const @baseName& self) throw(std::exception)
	{
		@dispatch(return os << *(const @className(@inClassName)*)(self.self_);)
	}
	friend inline std::istream& operator>>(std::istream& is, @baseName& self) throw(std::exception)
	{
		@dispatch(return is >> *(@className(@inClassName)*)(self.self_);)
	}"""

def dispatchLine(fo, sp, code, baseName, friend):
	if friend:
		fpType = 'self.fpType_'
	else:
		fpType = 'fpType_'
	print>>fo, sp + 'switch (%s) {' % fpType
	i = 0
	for type in TYPE_NUM_TBL:
		def replaceFunc(p):
			return BASE_NAME % (type, p.group(1))
		str = RE_CLASSNAME.sub(replaceFunc, code)
		print>>fo, sp + 'case %d:' % i, str, 'break;'
		i += 1
	print>>fo, sp + 'default: throw cybozu::Exception("elgamal_disp:%s:bad fpType_") << %s << __FILE__ << __LINE__;' % (baseName, fpType)
	print>>fo, sp + '}'

def replaceBaseFeature(replaced, baseFeature, baseName, inClassName):
	for line in baseFeature.split('\n'):
		str = line.replace('@baseName', baseName)
		str = str.replace('@inClassName', inClassName)
		replaced.append(str + '\n')


def dispatch(inName, outName):
	fi = open(inName, "r")
	fo = open(outName, "w")

	friend = False
	replaced = []
	for line in fi:
		p = RE_BASE_FEATURE.search(line)
		if p:
			replaceBaseFeature(replaced, BASE_FEATURE, p.group(1), p.group(2))
			continue
		p = RE_BASE_FEATURE_NOSTR.search(line)
		if p:
			replaceBaseFeature(replaced, BASE_FEATURE_NOSTR, p.group(1), p.group(2))
			continue
		replaced.append(line)

	baseName = ''
	for line in replaced:
		p = RE_SET_BASENAME.search(line)
		if p:
			baseName = p.group(1)
			continue
		p = RE_DISPATCH.match(line)
		if p:
			sp = p.group(1)
			code = p.group(2)
			dispatchLine(fo, sp, code, baseName, friend)
		else:
			print>>fo, line,
			if line.find('}') > 0:
				friend = False
			elif line.find('friend inline') >= 0:
				friend = True

def main():
	if len(sys.argv) != 3:
		print 'usage : dispatch.py input-file output-file'
	dispatch(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
    main()
