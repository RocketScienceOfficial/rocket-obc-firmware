"""
Script which helped refactoring whole codebase
"""


import re
import json
import os
from glob import glob

PATH = r""
PATH_MAPPINGS = "refactor_mappings.json"
CUSTOM_MODULES_MAPPINGS = {
    "LSM6DSO32": "lsm6dso32",
    "MAX1161X": "max1161x",
    "NMEA": "nmea",
    "UBloxNeoM9N": "ublox_neo_m9n",
    "uBloxNeoM9N": "ublox_neo_m9n",
    "UBloxSamM10Q": "ublox_sam_m10q",
    "uBloxSamM10Q": "ublox_sam_m10q",
    "E19_433M20SC": "e19_433m20sc",
    "SX126X": "sx126x",
    "sx126X": "sx126x",
    "SX127X": "sx127x",
    "sx127X": "sx127x",
    "MMC5983MA": "mmc5983ma",
}
CUSTOM_NAMES = [
    "I2C",
    "SPI",
    "UART",
    "PWM",
    "GPIO",
    "ODR",
    "OSR",
    "HPFc",
    "_GGA",
    "_GBS",
    "_GLL",
    "_GNS",
    "_GSA",
    "_GST",
    "_RMC",
    "_VTG",
    "_ZDA",
    "CRC",
    "GFSK",
    "RX",
    "TX",
    "IRQ",
    "TCXO",
    "SF",
    "BW",
    "CR",
    "CAD",
    "LoRa",
    "CMD",
    "RTC",
    "RF",
    "DIO",
    "FS",
    "RESET",
    "SET",
    "IMU",
    "MARG",
]


def convert_typedef_name(name: str):
    custom_name = True

    for x in CUSTOM_MODULES_MAPPINGS:
        if custom_name:
            if name.find(x) != -1:
                custom_name = False

        name = name.replace(x, CUSTOM_MODULES_MAPPINGS[x].lower())

    for x in CUSTOM_NAMES:
        name = name.replace(x, ("_" if "_" not in x else "") + x.lower())

    new_name = ""
    was_numeric = False

    for x in name:
        if x.isnumeric():
            was_numeric = True
            custom_name = True
        else:
            if was_numeric:
                custom_name = False

        if x.isupper():
            if not custom_name:
                new_name += "_"

            new_name += x.lower()
        else:
            new_name += x

    return new_name


def convert_function_name(name: str):
    for x in CUSTOM_MODULES_MAPPINGS:
        name = name.replace(x, CUSTOM_MODULES_MAPPINGS[x].lower())

    for x in CUSTOM_NAMES:
        name = name.replace(x, ("_" if "_" not in x else "") + x.lower())

    new_name = ""

    for i in range(len(name)):
        if name[i].isupper():
            new_name += "_" + name[i].lower()
        else:
            new_name += name[i]

    return new_name


content = ""
mappings = {}

with open(PATH_MAPPINGS, "r") as f:
    mappings = json.load(f)

result = [y for x in os.walk(PATH) for y in glob(os.path.join(x[0], "*.c"))]

for path in result:
    with open(path, "r") as f:
        content = f.read()
        for m in mappings:
            content = re.sub(f"\\b{m}\\b", mappings[m], content)

        file_name = PATH.split("\\")[-1]
        is_header = PATH.split(".")[1] == "h"
        has_guard = False

        if is_header:
            header_guard_name = "_" + file_name.split(".")[0].upper() + "_H"

            content = content.replace(
                "#pragma once",
                f"#ifndef {header_guard_name}\n#define {header_guard_name}",
            )

            has_guard = "#endif" in content

        content = content.replace("FUNCRESULT", "void")
        content = content.replace("SPIInstance", "spi_instance_t")
        content = content.replace("I2CInstance", "i2c_instance_t")
        content = content.replace("ADCInput", "adc_input_t")
        content = content.replace("PinNumber", "pin_number_t")
        content = content.replace("BOOL", "bool")
        content = content.replace("BYTE", "uint8_t")
        content = content.replace("UINT8", "uint8_t")
        content = content.replace("INT8", "int8_t")
        content = content.replace("UINT16", "uint16_t")
        content = content.replace("INT16", "int16_t")
        content = content.replace("UINT32", "unsigned int")
        content = content.replace("INT32", "int")
        content = content.replace("UINT64", "unsigned long long")
        content = content.replace("INT64", "long")
        content = content.replace("VOID", "void")
        content = content.replace("SIZE8", "uint8_t")
        content = content.replace("SIZE", "size_t")
        content = content.replace("FLOAT", "float")
        content = content.replace("DOUBLE", "double")
        content = content.replace("CHAR", "char")
        content = content.replace("STRING", "char*")
        content = content.replace("vec3", "vec3_t")
        content = content.replace("quat", "quat_t")
        content = content.replace("__", "_")
        content = content.replace("\n * @return Result code", "")
        content = content.replace("\n\n    return SUC_OK;", "")
        content = content.replace("\n#include <obc/api.h>", "")
        content = content.replace(
            '#include "drivers/tools/time_tracker.h"',
            '#include "modules/drivers/hal/time_tracker.h"',
        )
        content = content.replace(
            '#include "maths/constants.h"', '#include "modules/maths/math_constants.h"'
        )
        content = content.replace(
            '#include "maths/math_utils.h"', '#include "modules/maths/math_utils.h"'
        )
        content = content.replace(
            '#include "maths/vector.h"', '#include "modules/maths/vector.h"'
        )
        content = content.replace(
            '#include "maths/quaternion.h"', '#include "modules/maths/quaternion.h"'
        )
        content = content.replace('"drivers', '"modules/drivers')
        content = content.replace("/gpio/", "/hal/")

        for m in mappings:
            content = re.sub(f"\\b{m}\\b", mappings[m], content)

        if is_header:
            tmp = ""
            is_typedef_begin = False
            is_typedef_end = False
            is_function = False

            for c in content:
                tmp += c

                if tmp == "typedef struct " or tmp == "typedef enum ":
                    tmp = ""
                    is_typedef_begin = True
                elif tmp == "} ":
                    tmp = ""
                    is_typedef_end = True
                elif (
                    tmp == "void "
                    or tmp == "bool "
                    or tmp == "uint8_t "
                    or tmp == "float "
                    or tmp == "unsigned int "
                ):
                    tmp = ""
                    is_function = True

                if c == "\n":
                    if is_typedef_begin:
                        content = content.replace(tmp, convert_typedef_name(tmp), 1)
                        is_typedef_begin = False

                    tmp = ""
                elif c == ";":
                    if is_typedef_end:
                        tmp = tmp[:-1]
                        mappings[tmp] = convert_typedef_name(tmp) + "_t"
                        content = re.sub(f"\\b{tmp}\\b", mappings[tmp], content)
                        is_typedef_end = False

                    tmp = ""
                elif c == "(":
                    if is_function:
                        tmp = tmp[:-1]
                        mappings[tmp] = convert_function_name(tmp)
                        content = re.sub(f"\\b{tmp}\\b", mappings[tmp], content)
                        is_function = False

                    tmp = ""

            if "bool" in content and not "<stdbool.h>" in content:
                ind = content.rfind("#include")

                if ind != -1:
                    size = 0

                    for i in range(ind, len(content)):
                        if content[i] == "\n":
                            break

                        size += 1

                    content = content.replace(
                        content[ind : ind + size],
                        content[ind : ind + size] + "\n#include <stdbool.h>",
                    )
            if (
                "int8_t" in content or "int16_t" in content
            ) and not "<stdint.h>" in content:
                ind = content.rfind("#include")

                if ind != -1:
                    size = 0

                    for i in range(ind, len(content)):
                        if content[i] == "\n":
                            break

                        size += 1

                    content = content.replace(
                        content[ind : ind + size],
                        content[ind : ind + size] + "\n#include <stdint.h>",
                    )
            if "size_t" in content and not "<stddef.h>" in content:
                ind = content.rfind("#include")

                if ind != -1:
                    size = 0

                    for i in range(ind, len(content)):
                        if content[i] == "\n":
                            break

                        size += 1

                    content = content.replace(
                        content[ind : ind + size],
                        content[ind : ind + size] + "\n#include <stddef.h>",
                    )

            if not has_guard:
                content += "\n\n#endif"

    print(path)

    with open(path, "w") as f:
        f.write(content)

with open(PATH_MAPPINGS, "w") as f:
    new_mappings = {}

    for m in mappings:
        new_mappings[mappings[m]] = "hal_" + mappings[m]

    json.dump(new_mappings, f, ensure_ascii=False, indent=4)
