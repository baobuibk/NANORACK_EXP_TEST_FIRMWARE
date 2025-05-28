# Command-Line Interface Usage Guide

This document provides instructions for using the command-line interface to control and monitor laser operations. The interface supports three commands: `help`, `set_laser`, and `get_current`. Below are the details for each command, including their format, parameters, and usage examples.

## Commands

### 1. `help`
Displays the list of available commands and their formats.

- **Format**: `help`
- **Description**: Lists all supported commands with their syntax and brief descriptions.
- **Example**:
  ```
  help
  ```
  Output: Displays the command table with available commands and their formats.

### 2. `set_laser`
Controls the on/off state and DAC (Digital-to-Analog Converter) value for a specific laser, either onboard or external.

- **Format**: `set_laser [int/ext] [laser_index] [dac_val]`
- **Parameters**:
  - `[int/ext]`: Specifies the laser region. Use `int` for onboard lasers or `ext` for external lasers.
  - `[laser_index]`: The index of the laser on the board, starting from 1. Use `0` to turn off all lasers in the specified region.
  - `[dac_val]`: The DAC value, ranging from `0` to `100`, corresponding to a voltage range of `0` to `3.3V`. This parameter is optional when turning off all lasers (i.e., when `laser_index` is `0`).
- **Description**: Turns on a specific laser with the specified DAC value or turns off all lasers in the selected region.
- **Examples**:
  ```
  set_laser int 1 10
  ```
  Turns on onboard laser 1 with a DAC value of 10% (0.33V).
  ```
  set_laser int 0
  ```
  Turns off all onboard lasers.
  ```
  set_laser ext 1 10
  ```
  Turns on external laser 1 with a DAC value of 10% (0.33V).
  ```
  set_laser ext 0
  ```
  Turns off all external lasers.

### 3. `get_current`
Reads the current flowing through the lasers in the specified region.

- **Format**: `get_current [int/ext]`
- **Parameters**:
  - `[int/ext]`: Specifies the laser region. Use `int` for onboard lasers or `ext` for external lasers.
- **Description**: Returns the current flowing through the laser in the specified region, measured in milliamperes (mA).
- **Examples**:
  ```
  get_current int
  ```
  Returns the current flowing through onboard lasers in mA.
  ```
  get_current ext
  ```
  Returns the current flowing through external lasers in mA.

## Notes
- Ensure that the `laser_index` is valid for the specific board configuration.
- The `dac_val` must be between `0` and `100`. Values outside this range may cause errors.
- When using `set_laser` with `laser_index` set to `0`, the `dac_val` parameter is not required.
- The `laser_index` must be in the range of `0` to `36` for onboard lasers (`int`) and `0` to `8` for external lasers (`ext`). Values outside these ranges will cause errors.
- Commands are case-sensitive and must follow the exact format specified.

For further assistance, use the `help` command to review the available commands and their syntax.