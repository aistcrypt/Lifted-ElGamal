import java.io.*;
import jp.go.aist.risec.elgamal.*;

/*
	ElgamalText [ecParam]
	ecParam = secp192k1, NIST_P224, ...
	hashParam = hash224, hash384, ...
*/
public class ElgamalTest {
	static {
		System.loadLibrary("elgamal_if_wrap");
	}
	public static void assertEquals(String msg, int x, int y) {
		if (x == y) {
			System.out.println("OK : " + msg);
		} else {
			System.out.println("NG : " + msg + ", x = " + x + ", y = " + y);
		}
	}
	public static void main(String argv[]) {
		try {
			String ecStr = "secp192k1";
			String hashStr = "sha224";
			for (int i = 0; i < argv.length; i++) {
				if (argv[i].equals("-e") && i < argv.length - 1) {
					ecStr = argv[i + 1];
					i++;
				} else
				if (argv[i].equals("-h") && i < argv.length - 1) {
					hashStr = argv[i + 1];
					i++;
				}
			}
			String param = ecStr + " " + hashStr;
			System.out.println("param=" + param);
			Elgamal.SystemInit(param);

			String prvStr = "";
			String pubStr = "";
			{
				PrivateKey prv = new PrivateKey();
				prv.init();
				prvStr = prv.toStr();
				PublicKey pub = prv.getPublicKey();
				pubStr = pub.toStr();
			}
			int m = 1234;
			CipherText c = new CipherText();
			PublicKey pub = new PublicKey();

			pub.fromStr(pubStr);

			pub.enc(c, m);

			PrivateKey prv = new PrivateKey();
			prv.fromStr(prvStr);

			int dec = prv.dec(c);
			// verify dec(enc(m)) == m
			assertEquals("dec(enc(m)) == m", m, dec);

			// verify toStr, fromStr
			{
				String cStr = c.toStr();
				CipherText c2 = new CipherText();
				c2.fromStr(cStr);
				int dec2 = prv.dec(c2);
				assertEquals("fromStr(toStr(CipherText) == CipherText", dec, dec2);
			}

			// verify dec(enc(str)) == str
			pub.enc(c, "1234");
			dec = prv.dec(c);
			assertEquals("dec(enc(str)) == str", m, dec);

			// verify dec(mul(enc(m), 3)) == m * 3
			c.mul(3);
			m *= 3;
			dec = prv.dec(c);
			assertEquals("mul(int)", m, dec);

			// verify dec(mul(enc(m), "10")) == m * 10
			c.mul("10");
			m *= 10;
			dec = prv.dec(c);
			assertEquals("mul(str)", m, dec);

			// convert str
			{
				String s = c.toStr();
				CipherText c2 = new CipherText();
				c2.fromStr(s);
				dec = prv.dec(c);
				assertEquals("fromStr", m, dec);
			}
			// rerandomize
			pub.rerandomize(c);
			dec = prv.dec(c);
			assertEquals("rerandomize", m, dec);
			int m2 = 12345;
			// verify dec(add(enc(m), m2)) == m + m2
			pub.add(c, m2);
			m += m2;
			dec = prv.dec(c);
			assertEquals("pub.add(int)", m, dec);

			pub.add(c, "993");
			m += 993;
			dec = prv.dec(c);
			assertEquals("pub.add(str)", m, dec);

			// string test
			String m3 = "-2000000";
			String m4 =  "2001234";
			CipherText c2 = new CipherText();
			pub.enc(c, m3);
			pub.enc(c2, m4);
			c.add(c2); // m3 + m4

			dec = prv.dec(c);
			assertEquals("int add", 1234, dec);

		} catch (RuntimeException e) {
			System.out.println("unknown exception :" + e);
		}
	}
}
