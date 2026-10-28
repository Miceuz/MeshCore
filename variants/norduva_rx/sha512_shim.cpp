// Route orlp ed25519's sha512 to rweather SHA512, so only one SHA512 is linked.
#ifdef ED25519_EXTERNAL_SHA512

#include <new>
#include <SHA512.h>
#include <sha512.h>

static_assert(sizeof(SHA512) <= sizeof(sha512_context), "sha512_context too small");

extern "C" int sha512_init(sha512_context* md) {
  new (md->opaque) SHA512();
  return 0;
}

extern "C" int sha512_update(sha512_context* md, const unsigned char* in, size_t inlen) {
  reinterpret_cast<SHA512*>(md->opaque)->update(in, inlen);
  return 0;
}

extern "C" int sha512_final(sha512_context* md, unsigned char* out) {
  SHA512* h = reinterpret_cast<SHA512*>(md->opaque);
  h->finalize(out, 64);
  h->~SHA512();
  return 0;
}

extern "C" int sha512(const unsigned char* message, size_t message_len, unsigned char* out) {
  sha512_context ctx;
  sha512_init(&ctx);
  sha512_update(&ctx, message, message_len);
  return sha512_final(&ctx, out);
}

#endif
