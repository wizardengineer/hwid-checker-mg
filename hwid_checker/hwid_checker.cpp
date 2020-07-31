// hwid_checker.cpp

#include "includes/smbios.hpp"

using std::cout, std::vector;
using namespace smbios;

//
// streaming the GUID to std::cout
// operatoroverloading will take place
//

auto operator<<(
	std::ostream& os, 
	GUID adapter
	) 
	-> std::ostream&
{
	os << std::uppercase;
	os.width(8); 
	
	//
	// first  set of 8 characters in the GUID
	// for example: F54F83C5
	//

	os << std::hex << adapter.Data1 << '-';
	
	os.width(4);

	//
	// second set of 4 characters in the GUID
	// for example: 9724
	//

	os << std::hex << adapter.Data2 << '-';

	os.width(4);

	//
	// third set of 4 characters in the GUID
	// for example: 6A1C
	//

	os << std::hex << adapter.Data3 << '-';
	
	os.width(2);
	os << std::hex
		<< static_cast<short> (adapter.Data4[0]) // fourth set of characters
		<< static_cast<short> (adapter.Data4[1]) //
		<< '-'
		<< static_cast<short> (adapter.Data4[2]) // fifth set of characters
		<< static_cast<short> (adapter.Data4[3]) //
		<< static_cast<short> (adapter.Data4[4])
		<< static_cast<short> (adapter.Data4[5])
		<< static_cast<short> (adapter.Data4[6])
		<< static_cast<short> (adapter.Data4[7]);
	os << std::nouppercase;
	return os;

}

extern "C" {
	IDirect3D9 *Direct3DCreate9(
		UINT SDKVersion
	);
}

auto main()
	-> int
{
	DWORD hdd_serial							 {};
	DWORD flag										 {1};
	DWORD gpu_serial							 {};
	DWORD smbios_data_size				 {};
	BYTE  *b_					             {nullptr};
	dmi_header* header             {nullptr};
	RawSMBIOSData* smbios_data     {nullptr};
  BYTE* smbios_data_buffer; //[0x10000] = { 0 }; //0.065536 megabytes
	smbios_data_buffer = new BYTE[0x10000]; //0.065536 megabytes fails

	// HW_PROFILE_INFO hw_prof_info won't be needed
	
	std::puts("\tGetting HWID\n");

	if (GetVolumeInformationA("C://", NULL, NULL,
		&hdd_serial, NULL, NULL, NULL, NULL) == 0) {
		cout << "Error encounters 'GetVolumeInformation: "
			<< GetLastError() << "\n";
		_getch();
	}

	//
	// SMBIOS (RSMB) firmware tables 
	//

	smbios_data_size = GetSystemFirmwareTable('RSMB', 0, 0, 0);

	if (!smbios_data_size) {
		std::puts("Error: ERROR_OUTOFMEMORY");
		_getch();
		ExitProcess(EXIT_FAILURE);
	}

	if (smbios_data_size == 0) {
		cout << "Error encounters 'GetSystemFirmwareTable': "
			<< GetLastError() << "\n";
		_getch();
		ExitProcess(EXIT_FAILURE);
	}
	int smbios_buffersize = smbios_data_size;

	smbios_data_size = GetSystemFirmwareTable('RSMB', 0, smbios_data_buffer, smbios_buffersize);
	
	if (!smbios_data_size) {
		std::puts("Error: ERROR_OUTOFMEMORY");
		_getch();
		ExitProcess(EXIT_FAILURE);
	}

	cout << "\tget buffer size is: " << smbios_buffersize << "\n";

	smbios_data =
		reinterpret_cast<RawSMBIOSData*> (
			smbios_data_buffer
			);


	if (smbios_data->Length != smbios_buffersize - 8)
	{
		printf("Smbios length error\n");
		_getch();
		ExitProcess(EXIT_FAILURE);
	}
	

	IDirect3D9 *d9object = Direct3DCreate9(D3D_SDK_VERSION);
	unsigned int adaptercount = d9object->GetAdapterCount();
	D3DADAPTER_IDENTIFIER9* adapters = new D3DADAPTER_IDENTIFIER9[sizeof(adaptercount)];

	for (unsigned int i = 0; i < adaptercount; i++) {
		d9object->GetAdapterIdentifier(i, 0, &(adapters[i]));
	}

	cout << "\tGPU desc:   " << adapters->Description << "\n";
	cout << "\tGPU vendor: " << adapters->VendorId << "\n";
	cout << "\tGPU guid:   " << adapters->DeviceIdentifier << "\n";
	cout << "\tHDD hwid:   " << hdd_serial << "\n";
	
	// eax, ebx, ecx, edx
	int cpuid[4] = { 0,0,0,0 };
	__cpuid(cpuid, 0);
	char16_t hold = 0;
	char16_t* pointer = reinterpret_cast<char16_t*> (cpuid);
	for (char32_t i = 0; i < 8; i++) {
		hold += pointer[i];
	}

	cout << "\tCPU   id:   " << std::uppercase << hold << "\n";

	// Go throught BIOS structures
	b_ = smbios_data->SMBIOSTableData;
	for (DWORD index{}; index < smbios_data->Length; index++) {
		
		dmi_header *header =
			reinterpret_cast<dmi_header*> (b_);

		if (header->type == dmi_entry_type::DMI_ENTRY_BIOS && flag) {
			std::printf("\n\tType  %02d - [Bios Device type]\n", header->type);
			cout << "\t\tBIOS Vendor:    " << dmi_string(header, b_[0x4]) << "\n";
			cout << "\t\tBIOS Version:   " << dmi_string(header, b_[0x5]) << "\n";
			cout << "\t\tRelease data:   " << dmi_string(header, b_[0x8]) << "\n";

			flag = 0;
		} 

		else if (header->type == dmi_entry_type::DMI_ENTRY_BASEBOARD) {
			std::printf("\n\tType  %02d - [Baseboard Device type]\n", header->type);
			cout << "\t\tManufacturer:   " << dmi_string(header, b_[0x4]) << "\n";
			cout << "\t\tProduct Number: " << dmi_string(header, b_[0x5]) << "\n";
			cout << "\t\tVersion:        " << dmi_string(header, b_[0x6]) << "\n";
			cout << "\t\tSerial Number:  " << dmi_string(header, b_[0x7]) << "\n";
			cout << "\t\tUUID:           "; 
			
			dmi_system_uuid(b_ + 0x8,
				smbios_data->SMBIOSMajorVersion * 0x100 + smbios_data->SMBIOSMinorVersion);


		} 
		

		else if (header->type == dmi_entry_type::DMI_ENTRY_MEM_DEVICE) {
			std::printf("\n\tType  %02d - [Memory Device type]\n", header->type);
			cout << "\t\tMemory Type:    " << dmi_memory_device_type(b_[0x12]) << "\n";
			cout << "\t\tSize of RAM:    " << dmi_string(header, b_[0xC]) << "\n";
			cout << "\t\tManufacturer:   " << dmi_string(header, b_[0x17]) << "\n";
			cout << "\t\tSerial Number:  " << dmi_string(header, b_[0x18]) << "\n";
		}

		
		b_ += header->length;
		while ((*(WORD*)b_) != 0) { b_++; }
		b_ += 2;
	}

	_getch();
	delete [] smbios_data_buffer;
	return {};
}
