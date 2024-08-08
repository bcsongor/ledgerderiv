# 🔏 ledgerderiv

> _[tinyvanityeth](https://github.com/bcsongor/tinyvanityeth) for Ledger hardware wallets!_

Derive Ethereum addresses with Ledger hardware wallets using custom paths and address matching to find vanity addresses.

_ledgerderiv_ connects directly to the hardware wallet via the USB HID interface communicating with the Ethereum application on the Ledger using APDU messages.

_ledgerderiv_ provides a Lua scripting environment to configure address derivation rules (see [`scripts`](scripts) for examples).

## Usage

As of now _ledgerderiv_ must be compiled from sources. It has been tested on macOS and Windows.

### Building

#### Prerequisites

- A C17 compiler (e.g. Clang or MSVC)
- CMake 3.26+
- [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd#1---set-up-vcpkg) (make sure to set `VCPKG_ROOT` to the correct path)

#### Building on macOS (_Apple Silicon, Clang_)

1. Generate the project files:
   ```shell
   cmake --preset macos-aarch64-release -B cmake-build-macos-aarch64-release
   ```
2. Build _ledgerderiv_:
   ```shell
   cmake --build cmake-build-macos-aarch64-release --config Release
   ```

#### Building on Windows (_x64, MSVC 2022_)

1. Generate the project files:
   ```shell
   cmake --preset windows-x64-release -B cmake-build-windows-x64-release
   ```
2. Build _ledgerderiv_:
   ```shell
   cmake --build cmake-build-windows-x64-release --config Release
   ```

To build on different platforms or configurations please see `CMakePresets.json` file.

### Running

```shell
Usage: ledgerderiv <OPTIONS>

Options: -s <script>  Lua script with the following functions defined:
                        NextPath()                         Returns the next HD derivation path
                        OnAddress(path, checksum_address)  Called after an address is derived
                        OnExit()                           Called when the program exits
         -h           Prints usage

Example: ledgerderiv -s scripts/find-vanity-address.lua
```


