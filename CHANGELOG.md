# Changelog

All notable changes to this project are documented in this file. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- Smoke server with PING, TOOL and ASSET verbs.
- Vendored zlib, bzip2, lz4 and cJSON as static libraries.

### Security

- 2026-02-05: Upgraded the vendored cJSON from 1.7.16 to 1.7.17 (CVE-2023-50472, out-of-bounds read in cJSON_SetValuestring).
- 2026-02-20: Fixed command injection in `run_tool` (CWE-78); tool names are now matched against a fixed list.
