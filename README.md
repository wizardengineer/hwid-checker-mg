![Build status](https://ci.appveyor.com/api/projects/status/hjxm9hfjwljab2am?svg=true)
# hwid-checker-mg

Before I proceed, I would like to give a huge thanks to nealhow_ ([github](https://github.com/nealhow), [twitter](https://twitter.com/nealhow_)) for being a huge help, by 
contributing so much with his knowlegde and skills. Without him and his guidence, this project would've took longer to make. 
Fun fact, at the time of making this project I wasn't able to find a single C++ hwid checker

### INTRODUCTION 

hwid-checker-mg is simple, proof-of-concept, hardware id checker made in C++ that utilizes the SMBIOS/DMI standards to 
output information that's been described by the BIOS. The information ranges from 
system manufacturer, model name, serial number (Which is our main focus for this hwid checker)

C++ has been chosen as the language of this project for two reasons:
- I couldn't, at time of making this, find a straight forward answer documentation for creating extracting data from the smbios.
  Most if the answers I've encounter were a bit vague. So, I created this to have a basic foundation on understand how HWID spoofers work
- It was a challenge

This was a great learning experience and was much a challenge. Here are some of the concepts
I've encountered 
- what is smbios and why was it needed?
  - The SMBIOS stands for *System Management BIOS*. It provides a set of data structures or tables that describes a computer's BIOS configuration.
	The use of SMBIOS elimiated the use of having to directly get the hardware's information.

- what is DMI?
  - Sort for Desktop Management Interface, is a manager that generates a standard for managing and tracking components of a computer.
    By standard it's referring to it being applicable to other OSes.

Evidently, it'll be need to read [System Management BIOS (SMBIOS) Reference Specification Version 3.3.0](https://www.dmtf.org/standards/smbios) to further your understanding on this topic.

Also, allocating a ``RawSMBIOSData data struct`` in the heap at runtime gave undefine behavior. So I wouldn't recommended it.

### DOCUMENTATION

**Doc 1: Table 3 – Structure header format description**

```c

// windows struct

typedef struct _dmi_header
{
	BYTE		type;
	BYTE		length;
	WORD		handle;
} dmi_header;

// and for linux

struct dmi_header {
	u8 type;
	u8 length;
	u16 handle;
} __packed;
```

- First, The type data member of size ``BYTE`` or ``uint8_t`` holds the SMBIOS specification defines the DMI Types different types of elements a computer. For example, type 2, means that the record contains 
  "Base Board" Information. To have a better understand of the Types SMBIOS defines, here's a code representation in file [smbios.hpp](https://github.com/medievalghoul/hwid-checker-mg/tree/master/hwid_checker/includes/smbios.hpp):

```c++
  enum dmi_entry_type
    : BYTE /* 6.1.2 Structure header format */
  {
    DMI_ENTRY_BIOS = 0,
    DMI_ENTRY_SYSTEM,
    DMI_ENTRY_BASEBOARD,
    DMI_ENTRY_CHASSIS,
    DMI_ENTRY_PROCESSOR,
    DMI_ENTRY_MEM_CONTROLLER,
    DMI_ENTRY_MEM_MODULE,
    DMI_ENTRY_CACHE,
    DMI_ENTRY_PORT_CONNECTOR,
    DMI_ENTRY_SYSTEM_SLOT,
    DMI_ENTRY_ONBOARD_DEVICE,
    DMI_ENTRY_OEMSTRINGS,
    DMI_ENTRY_SYSCONF,
    DMI_ENTRY_BIOS_LANG,
    DMI_ENTRY_GROUP_ASSOC,
    DMI_ENTRY_SYSTEM_EVENT_LOG,
    DMI_ENTRY_PHYS_MEM_ARRAY,
    DMI_ENTRY_MEM_DEVICE,
    DMI_ENTRY_32_MEM_ERROR,
    DMI_ENTRY_MEM_ARRAY_MAPPED_ADDR,
    DMI_ENTRY_MEM_DEV_MAPPED_ADDR,
    DMI_ENTRY_BUILTIN_POINTING_DEV,
    DMI_ENTRY_PORTABLE_BATTERY,
    DMI_ENTRY_SYSTEM_RESET,
    DMI_ENTRY_HW_SECURITY,
    DMI_ENTRY_SYSTEM_POWER_CONTROLS,
    DMI_ENTRY_VOLTAGE_PROBE,
    DMI_ENTRY_COOLING_DEV,
    DMI_ENTRY_TEMP_PROBE,
    DMI_ENTRY_ELECTRICAL_CURRENT_PROBE,
    DMI_ENTRY_OOB_REMOTE_ACCESS,
    DMI_ENTRY_BIS_ENTRY,
    DMI_ENTRY_SYSTEM_BOOT,
    DMI_ENTRY_MGMT_DEV,
    DMI_ENTRY_MGMT_DEV_COMPONENT,
    DMI_ENTRY_MGMT_DEV_THRES,
    DMI_ENTRY_MEM_CHANNEL,
    DMI_ENTRY_IPMI_DEV,
    DMI_ENTRY_SYS_POWER_SUPPLY,
    DMI_ENTRY_ADDITIONAL,
    DMI_ENTRY_ONBOARD_DEV_EXT,
    DMI_ENTRY_MGMT_CONTROLLER_HOST,
    DMI_ENTRY_INACTIVE = 126,
    DMI_ENTRY_END_OF_TABLE = 127,
  };
```
- Second, the length data member of size ``BYTE`` or ``uint8_t`` holds the size of the 
  formatted area starting at type field. Formated area can also be referred to as the area with recorded information.
  The length is also used as a relative offset as you'll be seeing in ([smbios.cpp](),[hwid_checker.cpp]()).

- Third, the handle data member of size ``WORD`` or ``uint16_t`` is a unique identifier, which allows certain records to reference to each other.
  An example would be memory device referencing to memory conttroller and vice versa.

**Doc 2: Raw SMBIOS firmware table provider**

```c++
#include <windows.h>

typedef struct RawSMBIOSData
{
	BYTE    Used20CallingMethod;
	BYTE    SMBIOSMajorVersion;
	BYTE    SMBIOSMinorVersion;
	BYTE    DmiRevision;
	DWORD   Length;
	BYTE    SMBIOSTableData[];
} RawSMBIOSData;

DWORD smbios_data_size	       {};
BYTE  *b_		       {nullptr};
dmi_header* header             {nullptr};
RawSMBIOSData* smbios_data     {nullptr};
BYTE smbios_data_buffer[0x10000] = { 0 };

GetSystemFirmwareTable('RSMB', 0, smbios_data_buffer, smbios_buffersize);
```

The **GetSystemFirmwareTable** function retrieves specified firmware tables from a firmware tables provider.
in our case the Raw SMBIOS Firmware provider tables.

Also, allocating a ``RawSMBIOSData data_structure`` in the heap at runtime, gave undefined behaviors. So I wouldn't recommended it.

### Resource that undeceive my research
    Most all of them are bit vague:	
    https://www.youtube.com/watch?v=yX6caxDOPu0
	https://stackoverflow.com/questions/43473262/getting-the-motherboards-serial-number
	https://www.experts-exchange.com/videos/7444/What-is-WMI-and-how-it-can-be-used-to-get-hardware-and-software-information-from-remote-computers.html
	https://www.codeproject.com/Questions/437379/Cplusplus-source-code-to-get-CPU-number-motherboar
	https://docs.microsoft.com/en-us/windows/win32/sysinfo/system-information-functions?redirectedfrom=MSDN
	https://stackoverflow.com/questions/56935213/how-to-get-adapter-driver-version-in-directx12/56960922
	https://stackoverflow.com/questions/1090261/get-the-graphics-card-model
	https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dadapter-identifier9
	https://docs.microsoft.com/en-us/windows/win32/api/guiddef/ns-guiddef-guid
	https://stackoverflow.com/questions/1672677/print-a-guid-variable
	https://www.windows-commandline.com/get-serial-number-for-ram-motherboard-hard-disk/
	https://stackoverflow.com/questions/43473262/getting-the-motherboards-serial-number
	https://www.dmtf.org/standards/smbios
	https://gist.github.com/daiakushi/98a004e7cd750803dfe1
	https://www.codeproject.com/Tips/5263343/How-to-Get-the-BIOS-UUID


	by reading source code of dmicode:
	- https://github.com/mirror/dmidecode/blob/master/dmidecode.c
	- https://elixir.bootlin.com/linux/latest/source/include/linux/dmi.h

	uuid vs guid: 
	- https://stackoverflow.com/questions/246930/is-there-any-difference-between-a-guid-and-a-uuid?rq=1

	dmicode:
	- https://linux.die.net/man/8/dmidecode  
  
  
Thanks for the read
