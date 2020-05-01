#include <stdint.h>
#include <openssl/aes.h>
#include <openssl/modes.h>

void aesIgeDecryptRaw(
    const unsigned char *src,
    unsigned char *dst,
    size_t len,
    const unsigned char *key,
    unsigned char *iv) {
  AES_KEY aes;
  AES_set_decrypt_key(key, 256, &aes);
  AES_ige_encrypt(src, dst, len, &aes, iv, AES_DECRYPT);
}

static void incrementIv(unsigned char iv[16], uint64_t blockIndex)
{
  if (!blockIndex) {
    return;
  }
  int digits = 16;
  uint64_t increment = blockIndex;
  do {
    --digits;
    increment += iv[digits];
    iv[digits] = increment & 0xFFULL;
    increment >>= 8;
  } while (digits != 0 && increment != 0);
}

void my_CRYPTO_ctr128_encrypt(
    const unsigned char *src,
    unsigned char *dst,
    size_t len,
    const unsigned char *key,
    int keysize,
    uint64_t block_index,
    unsigned char *iv) {
  AES_KEY aes;
  AES_set_encrypt_key(key, keysize, &aes);
  unsigned char ecount_buf[16] = {0};
  unsigned int num = 0;
  incrementIv(iv, block_index);
  CRYPTO_ctr128_encrypt(
      src, dst, len,
      &aes, iv,
      ecount_buf,
      &num,
      (block128_f)AES_encrypt
  );
}
