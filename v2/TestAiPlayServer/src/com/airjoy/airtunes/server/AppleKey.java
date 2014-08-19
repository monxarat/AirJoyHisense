
package com.airjoy.airtunes.server;

import android.util.Base64;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.security.KeyFactory;
import java.security.PrivateKey;
import java.security.spec.PKCS8EncodedKeySpec;

import javax.crypto.Cipher;

public class AppleKey {

    private static final AppleKey mSingleInstance = new AppleKey();
    private static final String RASKEY = "MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQDn10TyouJ4i2wfVaCOtwVEqPp5RaqL5sYs5fUcvdTcaEL+PRCD3S7ewb/UJS3ALm85i98OYUjqhIVeLkQtptYmZPZ0ofMEkpreT2iT7y325xGox3oNkcnZgIIuUNEpIq/qQOqfDhTA92k4xfOIL8AyPdn+VRVfUbtZIcIBYp/XM1LV4u+qv5ugSNe4E6K2dn9sPM8etM5nPQN7DS6jDF//6wb40Ird5AlXGpxon+8QcohV3Yz7movvXIlD7ztfqhXd5pi+3fNZlgPrPm9hNyu2KPZVn1maeL9QBoeqf0l2wFYtQSlW+JieGKY1W9gVl4JeD8h1ND7HghF2Jc2/mER7AgMBAAECggEBAOXwDHL1d9YEuaTOQSKqhLAXQ+yZWs/Mf0qyfAsYf5BmW+NZ3xJZgY3u7XnTse+EXk3d2smhVTc7XicNjhMVABouUn1UzfkACldovJjURGs3u70Asp3YtTBiEzsqbnf07jJQViKQTacg+xwSwDmW2nE6BQYJjtvt7Pk20PqcvVkpq7Dto1eZUC+YlNy4/FaaiS0XeAMkorbDFm40ZwkTS4VAQbhncGtY/vKg25Ird2KLaOaWk8evQ78qc9C3Mjd6C6F7RPBR6b95hJ3LMzJXH9inCTPC1gvexHmTSj2spAu28vN8Cp0HEG6tyLNpoD8vQciACY6K3UYkDaxozFNU82ECgYEA9+C/Wh5nGDGai2IJwxcURARZ+XOFZhOxeuFQi7PmMW5rf0YtL31kQSuEt2vCPysMNWJFUnmyQ6n3MW+VgAezTGH3aOLUTtX/KycoF+wys+STkpIo+ueOd0yg9169adWSAnmPEW42DGQ4sy4b2LncHjIy8NMJGIg8xD743aIsNpECgYEA72//+ZTx5WRBqgA1/RmgyNbwI3jHBYDZxIQgeR30B8WR+26/yjIsMIbdkB/S+uGuu2St9rt5/4BRvr0M2CCriYdABgGnsv6TkMrMmsq47Sv5HRhtj2lkPX7+D11W33V3otA16lQT/JjY8/kI2gWaN52kscw48V1WCoPMMXFTyEsCgYEA0OuvvEAluoGMdXAjNDhOj2lvgE16oOd2TlB7t9Pf78fWeMZoLT+tcTRBvurnJKCewJvcO8BwnJEz1Ins4qUa3QUxJ0kPkobRc8ikBU3CCldcfkwMmDT0od6HSRej5ADq+IUGLbXLfjQ2iecR91/ng9fhkZL9dpzVQr6kuQEH7NECgYB/QBjcfeopLaUwQjhvMQWgd4rcbz3mkNordMUFWYPt9XRmGi/Xt96AU8zA4gjwyKxib1l9PZnSzlGjezmuS36e8sB18L89g8rNMtqWkZLCiZI1glwH0c0yWaGQbNzUmcthPiLJTLHqlxkGYJ3xsPSLBj8XNyA0NpSZtf35cO9EDQKBgQCQTukg+UTvWq98lCCgD16bSAgsC4Tg+7XdoqImd9+3uEiNsr7mTJvdPKxm+jIOdvcc4q8icru9dsq5TghKDEHZsHcdxjNAwazPWonaAbQ3mG8mnPDCFuFeoUoDjNppKvDrbbAOeIArkyUgTS0gAoo/jLE0aOgPZBiOEEa6G+RYpg==";

    private PrivateKey mPrivateKey = null;

    private AppleKey() {
    }

    public static AppleKey getInstance() {
        return mSingleInstance;
    }

    public PrivateKey getRsaKey() {
        if (null == mPrivateKey) {
            try {
                mPrivateKey = KeyFactory.getInstance("RSA").generatePrivate(
                        new PKCS8EncodedKeySpec(Base64.decode(RASKEY, Base64.DEFAULT)));
            } catch (Exception e) {
                e.printStackTrace();
                new AssertionError(e);
            }
        }

        return mPrivateKey;
    }
    
    public String getResponse(byte ip[], byte mac[], String challenge) {
        byte[] decoded = Base64.decode(challenge, Base64.DEFAULT);

        ByteArrayOutputStream out = new ByteArrayOutputStream();

        // Challenge
        try {
            out.write(decoded);
            out.write(ip);
            out.write(mac);

            // Pad to 32 Bytes
            int padLen = 32 - out.size();
            for (int i = 0; i < padLen; ++i)
                out.write(0x00);
        } catch (IOException e) {
            e.printStackTrace();
        }

        // RSA
        byte[] crypted = this.encryptRSA(out.toByteArray());

        // Encode64
        String resp = Base64.encodeToString(crypted, Base64.DEFAULT);

        // On retire les ==
        resp = resp.replace("=", "").replace("\r", "").replace("\n", "");

        return resp;
    }

    public byte[] encryptRSA(byte[] array) {
        try {
            Cipher cipher = Cipher.getInstance("RSA/NONE/PKCS1Padding");
            cipher.init(Cipher.ENCRYPT_MODE, AppleKey.getInstance().getRsaKey());
            return cipher.doFinal(array);

        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }

    public byte[] decryptRSA(byte[] array) {
        try {
            // La clef RSA
            // Encrypt
            Cipher cipher = Cipher.getInstance("RSA/NONE/OAEPPadding");
            cipher.init(Cipher.DECRYPT_MODE, AppleKey.getInstance().getRsaKey());
            return cipher.doFinal(array);

        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }
}
