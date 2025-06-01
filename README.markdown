# Command-Line Interface Usage Guide

This document provides instructions for using the command-line interface to control and monitor laser operations and photodiode sampling. The interface supports several commands for controlling lasers and acquiring data from photodiodes. Below are the details for each command, including their format, parameters, and usage examples.

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
  - `[laser_index]`: The index of the laser on the board, starting from 1. Use `0` to turn off all lasers in the specified region. Range: `0` to `36` for `int`, `0` to `8` for `ext`.
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

### 4. `pd_get`
Reads the ADC value from a specific photodiode using polling.

- **Format**: `pd_get [pd_index]`
- **Parameters**:
  - `[pd_index]`: The index of the photodiode on the board, ranging from `1` to `36`.
- **Description**: Reads the ADC value from the specified photodiode.
- **Example**:
  ```
  pd_get 20
  ```
  Reads the ADC value from photodiode 20.
- **Notes**:
  - The `pd_index` must be between `1` and `36`. Values outside this range will cause errors.

### 5. `sp_set`
Prepares the system for data sampling from a specific photodiode.

- **Format**: `sp_set [photo_index] [sampling_rate]`
- **Parameters**:
  - `[photo_index]`: The index of the photodiode to sample, ranging from `1` to `36`.
  - `[sampling_rate]`: The sampling rate in samples per second (SPS), ranging from `1` to `330000`.
- **Description**: Configures the system to sample data from the specified photodiode at the given sampling rate.
- **Example**:
  ```
  sp_set 10 300000
  ```
  Configures sampling for photodiode 10 at a rate of 300,000 SPS.
- **Notes**:
  - The `photo_index` must be between `1` and `36`.
  - The `sampling_rate` must be between `1` and `330000`. Values outside these ranges will cause errors.

### 6. `sp_trig`
Triggers data sampling and stores the samples in a temporary buffer.

- **Format**: `sp_trig [num_samples]`
- **Parameters**:
  - `[num_samples]`: The number of samples to collect, ranging from `1` to `50000`.
- **Description**: Initiates sampling and stores the specified number of samples in a temporary buffer.
- **Example**:
  ```
  sp_trig 50
  ```
  Triggers sampling of 50 samples and stores them in the buffer.
- **Notes**:
  - The `num_samples` must be between `1` and `50000`. Values outside this range will cause errors.

### 7. `sp_status`
Returns the status of the sampling process.

- **Format**: `sp_status`
- **Description**: Returns three parameters: the `pd_index` of the photodiode being sampled, the `sampling_rate` in SPS, and the buffer status (indicating whether the data in the buffer is ready to be retrieved using `sp_get` or `sp_get_c`).
- **Example**:
  ```
  sp_status
  ```
  Returns the photodiode index, sampling rate, and buffer status.
- **Notes**:
  - No parameters are required.

### 8. `sp_get`
Retrieves sampled data from the buffer and outputs it to the console in binary format.

- **Format**: `sp_get [num_samples]`
- **Parameters**:
  - `[num_samples]`: The number of samples to retrieve, ranging from `1` to `50000`.
- **Description**: Outputs an array of the specified number of samples in 16-bit binary format, followed by a 2-byte CRC to ensure data integrity.
- **Example**:
  ```
  sp_get 10
  ```
  Retrieves 10 samples from the buffer and outputs them in binary format (big_endian): [`header`(3byte)] [data] with `header` is [0xF000000 | data_len] (e.g., `{F0}{00}{03}{00}{08}{00}{08}{00}{11}{A7}{8C}`).
- **Notes**:
  - The `num_samples` must be between `1` and `50000`. Values outside this range will cause errors.

### 9. `sp_get_c`
Retrieves sampled data from the buffer and outputs it to the console in ASCII format.

- **Format**: `sp_get_c [num_samples]`
- **Parameters**:
  - `[num_samples]`: The number of samples to retrieve, ranging from `1` to `50000`.
- **Description**: Outputs an array of the specified number of samples in ASCII format (e.g., `1234 5678 ...`), followed by a 2-byte CRC to ensure data integrity.
- **Example**:
  ```
  sp_get_c 10
  ```
  Retrieves 10 samples from the buffer and outputs them in ASCII format (e.g., `1234 5678 ...`).
- **Notes**:
  - The `num_samples` must be between `1` and `50000`. Values outside this range will cause errors.

## Notes
- Ensure that the `laser_index` is valid for the specific board configuration (`0` to `36` for `int`, `0` to `8` for `ext`).
- The `dac_val` must be between `0` and `100`. Values outside this range may cause errors.
- When using `set_laser` with `laser_index` set to `0`, the `dac_val` parameter is not required.
- The `pd_index` and `photo_index` for photodiode-related commands (`pd_get`, `sp_set`) must be between `1` and `36`. Values outside this range will cause errors.
- The `sampling_rate` for `sp_set` must be between `1` and `330000` SPS.
- The `num_samples` for `sp_trig`, `sp_get`, and `sp_get_c` must be between `1` and `50000`. Values outside this range will cause errors.
- Commands are case-sensitive and must follow the exact format specified.

For further assistance, use the `help` command to review the available commands and their syntax.