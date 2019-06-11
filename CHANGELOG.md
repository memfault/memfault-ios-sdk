# Memfault iOS SDK Changelog

## v0.5.0

- Decreased nRF "packet receipt notification parameter" to 7 instead of 10, to
  avoid sporadic SD flash write queue overflows that would result in a
  "Operation Not Permitted" / 0x08 when doing OTA using latest iPhone models.

## v0.4.0

- Changed default server base URL to `https://api.memfault.com` and
  `https://ingress.memfault.com`.
- Added `kMFLTApiIngressBaseURL` configuration option, alongside the existing
  `kMFLTApiIBaseURL` to override the "ingress" server for development purposes.
