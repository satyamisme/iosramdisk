## `iosramdisk.sh` - iOS SSH Ramdisk Utility

**Purpose:**
`iosramdisk.sh` is a Bash script designed to create and boot SSH-enabled ramdisks for iOS devices. This allows for low-level device access, useful for various development, research, and troubleshooting tasks. It combines functionalities from various existing SSHRD scripts.

**Supported Platforms:**
*   macOS (Darwin)
*   Linux

**Prerequisites & Dependencies:**

1.  **Command-Line Tools:**
    The script expects the following tools to be present in platform-specific subdirectories (`./tools/macos/` for macOS, `./tools/linux/` for Linux) relative to the script's location. Ensure these tools are executable.
    *   `gaster`: DFU exploit tool.
    *   `irecovery`: For iDevice communication in DFU/recovery mode.
    *   `img4`: For IMG4 format manipulation.
    *   `img4tool`: Alternative IMG4 tool.
    *   `iBoot64Patcher`: For patching iBSS/iBEC.
    *   `KPlooshFinder`: KernelCache patch finder.
    *   `kerneldiff`: KernelCache diffing tool.
    *   `pzb`: Partial Zip Browser (for IPSW file extraction).
    *   `jq`: Command-line JSON processor.
    *   `sshpass`: For non-interactive SSH password input.
    *   `iproxy`: For proxying connections to the device.
    *   `gtar` (macOS only, typically GNU tar): For extracting tarballs.
    *   `hfsplus` (Linux only): For HFS+ filesystem manipulation.
    *   `PlistBuddy` (macOS uses `/usr/libexec/PlistBuddy` if available, or a `PlistBuddy` in PATH/tools folder; Linux requires it in PATH or tools folder): For Plist parsing.

2.  **System-Installed Tools:**
    Ensure these common utilities are available in your system's PATH:
    *   `curl`: For HTTP requests.
    *   `gzip`: For decompressing `.gz` files.
    *   Standard shell utilities (`bash`, `grep`, `sed`, `awk`, `head`, `date`, `mkdir`, `rm`, `mv`, `chmod`, `sleep`, `find`, `sort`).

3.  **Required Resources:**
    *   **SHSH Blobs:** Valid SHSH blobs for your target device and iOS version, named by the device's CPID (e.g., `0x8010.shsh`), must be placed in the `./other/shsh/` directory. (The script currently expects CPID without "0x", e.g. `8010.shsh`).
    *   **SSH Tarballs:** Pre-packaged SSH tarballs are required in the `./resources/sshtars/` directory. Expected files (compressed as `.gz`):
        *   `ssh.tar.gz` (Standard SSH tools)
        *   `atvssh.tar.gz` (For Apple TV devices)
        *   `t2ssh.tar.gz` (For T2 Macs)
    *   **Optional Boot Logo:** A custom boot logo in IM4P format can be placed at `./other/bootlogo.im4p`.

**Setup:**

1.  **Make the script executable:**
    ```bash
    chmod +x iosramdisk.sh
    ```
2.  **Ensure Tools and Resources:** Verify all required command-line tools are in their respective `./tools/macos/` or `./tools/linux/` folders and are executable. Confirm SHSH blobs and SSH tarballs are in their designated locations. Create the `./other/` and `./resources/` directories if they don't exist.

**Usage:**

```
./iosramdisk.sh [global_options] <command> [command_options]
```

**Global Options:**
*   `--debug`: Enables verbose debug logging. This flag can typically be placed before the main command.

**Commands:**

1.  **`help` or `--help`**
    Displays the help message.
    ```bash
    ./iosramdisk.sh help
    ```

2.  **`--version`**
    Displays the script's version.
    ```bash
    ./iosramdisk.sh --version
    ```

3.  **`create`**
    Creates a new SSH ramdisk.
    ```bash
    ./iosramdisk.sh create --device <DEVICE_ID> --version <IOS_VERSION> [--ipsw <PATH_TO_IPSW> | --ipsw-url <URL>] [other_options]
    ```
    *   `--device <DEVICE_ID>`: Target device identifier (e.g., `iPhone10,3`, `iPad8,1`). **Required.**
    *   `--version <IOS_VERSION>`: Target iOS version (e.g., `15.1`, `14.7.1`). **Required.**
    *   `--ipsw <PATH_TO_IPSW>` (Optional): Path to a local IPSW file. If provided without `--ipsw-url`, the script may still use ipsw.me to determine component paths unless `--ipsw-url` is also given.
    *   `--ipsw-url <URL>` (Optional): Direct URL to the IPSW file. If omitted and `--ipsw` is not used, the script attempts to fetch it from ipsw.me. This takes precedence if both IPSW sources are specified.
    *   `--key <BOOT_KEY>` (Optional): Decryption key for the main DMG.
    *   `--variant <VARIANT_NAME>` (Optional): Build variant (e.g., `Install`). Defaults to `Install`.

4.  **`boot`**
    Boots a previously created ramdisk.
    ```bash
    ./iosramdisk.sh boot [ramdisk_directory_path]
    ```
    *   `[ramdisk_directory_path]` (Optional): Path to a specific ramdisk directory (e.g., `created_ramdisks/iPhone10,3_15.1_timestamp/`). If omitted, the script attempts to boot the latest ramdisk found in the `$OUTPUT_DIR` (default: `created_ramdisks/`) directory.

5.  **`ssh`**
    Attempts to establish an SSH connection to the device (assumes a ramdisk with SSH tools has been successfully booted). Uses default credentials (root/alpine) and iproxy on local port 2222.
    ```bash
    ./iosramdisk.sh ssh
    ```

6.  **`clean`**
    Removes all created ramdisks from the `$OUTPUT_DIR` (default: `created_ramdisks/`) directory after user confirmation.
    ```bash
    ./iosramdisk.sh clean
    ```

**Typical Workflow Example:**

1.  Ensure your iOS device is in DFU mode.
2.  Create the ramdisk:
    ```bash
    ./iosramdisk.sh --debug create --device iPhone10,3 --version 15.1
    ```
3.  Once creation is complete, if the device is not in DFU mode (e.g. it rebooted after gaster), put it back into DFU mode.
4.  Boot the ramdisk:
    ```bash
    ./iosramdisk.sh --debug boot
    ```
    (Or specify the path if you have multiple created ramdisks: `./iosramdisk.sh boot created_ramdisks/iPhone10,3_15.1_20231027-123000/`)
5.  After the device appears to have booted into the ramdisk (text on screen, etc.), connect via SSH:
    ```bash
    ./iosramdisk.sh ssh
    ```

**Output:**
*   Created ramdisk packages are stored in subdirectories within the `$OUTPUT_DIR` (default: `./created_ramdisks/`).
*   Each subdirectory is named using the device model, iOS version, and a timestamp (e.g., `iPhone10,3_15.1_20231027-123000/`).
*   An `info.txt` file within each subdirectory details the components and creation information.

**Troubleshooting:**
*   **Tool Not Found Errors:** Ensure all tools listed under "Prerequisites" are correctly placed in the `./tools/macos/` or `./tools/linux/` subdirectories and are executable. Some tools like `PlistBuddy` on macOS might be expected at system paths first.
*   **SHSH Blob Not Found:** Verify that a valid `.shsh` file, named by your device's CPID (e.g., `8010.shsh` - note: no `0x` prefix in filename), exists in the `./other/shsh/` directory.
*   **ipsw.me API Errors / IPSW Download Failures:** Check your internet connection. The ipsw.me API might be temporarily unavailable, or the specified iOS version might not be available for your device. Try providing a direct `--ipsw-url` or a local `--ipsw` file.
*   **Permission Denied:** Ensure `iosramdisk.sh` and all tools are executable (`chmod +x <file>`).
*   **Booting Issues:** If the device doesn't boot into the ramdisk, double-check that the correct iOS version and device ID were used for creation and that the SHSH blob is valid for that combination. Re-verify DFU mode at each step if needed.
*   Enable debug output with the `--debug` flag for more detailed logs, which can help pinpoint where a process might be failing.
*   **SSH Connection Issues:** Ensure `iproxy` started correctly (check debug logs). Verify the device has booted to the ramdisk and SSH is running. Firewall or network configurations might also interfere with local connections.

This README provides a comprehensive guide to setting up and using `iosramdisk.sh`.
