/**

Copyright (C) 2013, Intel Corporation

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this 
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this 
  list of conditions and the following disclaimer in the documentation and/or 
  other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors may 
  be used to endorse or promote products derived from this software without 
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
  THE POSSIBILITY OF SUCH DAMAGE.

**/

#ifndef __FSP_HOB_H__
#define __FSP_HOB_H__

//
// HobType of EFI_HOB_GENERIC_HEADER.
// 
#define EFI_HOB_TYPE_MEMORY_ALLOCATION    0x0002
#define EFI_HOB_TYPE_RESOURCE_DESCRIPTOR  0x0003
#define EFI_HOB_TYPE_GUID_EXTENSION       0x0004
#define EFI_HOB_TYPE_UNUSED               0xFFFE
#define EFI_HOB_TYPE_END_OF_HOB_LIST      0xFFFF

///
/// Describes the format and size of the data inside the HOB. 
/// All HOBs must contain this generic HOB header.
/// 
typedef struct {
  ///
  /// Identifies the HOB data structure type.
  ///
  UINT16    HobType;
  ///
  /// The length in bytes of the HOB.
  ///
  UINT16    HobLength;
  ///
  /// This field must always be set to zero.
  ///
  UINT32    Reserved;
} EFI_HOB_GENERIC_HEADER;

///
/// Enumeration of memory types introduced in UEFI.
/// 
typedef enum {
  ///
  /// Not used.
  ///
  EfiReservedMemoryType,
  ///
  /// The code portions of a loaded application. 
  /// (Note that UEFI OS loaders are UEFI applications.)
  ///
  EfiLoaderCode,
  ///
  /// The data portions of a loaded application and the default data allocation
  /// type used by an application to allocate pool memory.
  ///
  EfiLoaderData,
  ///
  /// The code portions of a loaded Boot Services Driver.
  ///
  EfiBootServicesCode,
  ///
  /// The data portions of a loaded Boot Serves Driver, and the default data
  /// allocation type used by a Boot Services Driver to allocate pool memory.
  ///
  EfiBootServicesData,
  ///
  /// The code portions of a loaded Runtime Services Driver.
  ///
  EfiRuntimeServicesCode,
  ///
  /// The data portions of a loaded Runtime Services Driver and the default
  /// data allocation type used by a Runtime Services Driver to allocate pool memory.
  ///
  EfiRuntimeServicesData,
  ///
  /// Free (unallocated) memory.
  ///
  EfiConventionalMemory,
  ///
  /// Memory in which errors have been detected.
  ///
  EfiUnusableMemory,
  ///
  /// Memory that holds the ACPI tables.
  ///
  EfiACPIReclaimMemory,
  ///
  /// Address space reserved for use by the firmware.
  ///
  EfiACPIMemoryNVS,
  ///
  /// Used by system firmware to request that a memory-mapped IO region
  /// be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
  ///
  EfiMemoryMappedIO,
  ///
  /// System memory-mapped IO region that is used to translate memory
  /// cycles to IO cycles by the processor.
  ///
  EfiMemoryMappedIOPortSpace,
  ///
  /// Address space reserved by the firmware for code that is part of the processor.
  ///
  EfiPalCode,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

/// 
/// EFI_HOB_MEMORY_ALLOCATION_HEADER describes the
/// various attributes of the logical memory allocation. The type field will be used for
/// subsequent inclusion in the UEFI memory map.
/// 
typedef struct {
  ///
  /// A GUID that defines the memory allocation region's type and purpose, as well as
  /// other fields within the memory allocation HOB. This GUID is used to define the
  /// additional data within the HOB that may be present for the memory allocation HOB.
  /// Type EFI_GUID is defined in InstallProtocolInterface() in the UEFI 2.0
  /// specification. 
  /// 
  EFI_GUID              Name;

  ///
  /// The base address of memory allocated by this HOB. Type
  /// EFI_PHYSICAL_ADDRESS is defined in AllocatePages() in the UEFI 2.0
  /// specification.
  ///
  EFI_PHYSICAL_ADDRESS  MemoryBaseAddress;

  /// 
  /// The length in bytes of memory allocated by this HOB.
  /// 
  UINT64                MemoryLength;

  ///
  /// Defines the type of memory allocated by this HOB. The memory type definition
  /// follows the EFI_MEMORY_TYPE definition. Type EFI_MEMORY_TYPE is defined
  /// in AllocatePages() in the UEFI 2.0 specification.
  /// 
  EFI_MEMORY_TYPE       MemoryType;

  ///
  /// Padding for Itanium processor family
  ///
  UINT8                 Reserved[4];
} EFI_HOB_MEMORY_ALLOCATION_HEADER;

///
/// Describes all memory ranges used during the HOB producer 
/// phase that exist outside the HOB list. This HOB type 
/// describes how memory is used, not the physical attributes of memory.
///
typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_MEMORY_ALLOCATION.
  ///
  EFI_HOB_GENERIC_HEADER            Header;
  ///
  /// An instance of the EFI_HOB_MEMORY_ALLOCATION_HEADER that describes the
  /// various attributes of the logical memory allocation.
  ///
  EFI_HOB_MEMORY_ALLOCATION_HEADER  AllocDescriptor;
  //
  // Additional data pertaining to the "Name" Guid memory
  // may go here.
  //
} EFI_HOB_MEMORY_ALLOCATION;

///
/// The resource type.
/// 
typedef UINT32 EFI_RESOURCE_TYPE;

//
// Value of ResourceType in EFI_HOB_RESOURCE_DESCRIPTOR.
// 
#define EFI_RESOURCE_SYSTEM_MEMORY          0x00000000
#define EFI_RESOURCE_MEMORY_MAPPED_IO       0x00000001
#define EFI_RESOURCE_IO                     0x00000002
#define EFI_RESOURCE_FIRMWARE_DEVICE        0x00000003
#define EFI_RESOURCE_MEMORY_MAPPED_IO_PORT  0x00000004
#define EFI_RESOURCE_MEMORY_RESERVED        0x00000005
#define EFI_RESOURCE_IO_RESERVED            0x00000006
#define EFI_RESOURCE_MAX_MEMORY_TYPE        0x00000007

///
/// A type of recount attribute type.
/// 
typedef UINT32 EFI_RESOURCE_ATTRIBUTE_TYPE;

//
// These types can be ORed together as needed.
//
// The first three enumerations describe settings
//
#define EFI_RESOURCE_ATTRIBUTE_PRESENT              0x00000001
#define EFI_RESOURCE_ATTRIBUTE_INITIALIZED          0x00000002
#define EFI_RESOURCE_ATTRIBUTE_TESTED               0x00000004
//
// The rest of the settings describe capabilities
//
#define EFI_RESOURCE_ATTRIBUTE_SINGLE_BIT_ECC           0x00000008
#define EFI_RESOURCE_ATTRIBUTE_MULTIPLE_BIT_ECC         0x00000010
#define EFI_RESOURCE_ATTRIBUTE_ECC_RESERVED_1           0x00000020
#define EFI_RESOURCE_ATTRIBUTE_ECC_RESERVED_2           0x00000040
#define EFI_RESOURCE_ATTRIBUTE_READ_PROTECTED           0x00000080
#define EFI_RESOURCE_ATTRIBUTE_WRITE_PROTECTED          0x00000100
#define EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTED      0x00000200
#define EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE              0x00000400
#define EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE        0x00000800
#define EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE  0x00001000
#define EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE     0x00002000
#define EFI_RESOURCE_ATTRIBUTE_16_BIT_IO                0x00004000
#define EFI_RESOURCE_ATTRIBUTE_32_BIT_IO                0x00008000
#define EFI_RESOURCE_ATTRIBUTE_64_BIT_IO                0x00010000
#define EFI_RESOURCE_ATTRIBUTE_UNCACHED_EXPORTED        0x00020000

///
/// Describes the resource properties of all fixed, 
/// nonrelocatable resource ranges found on the processor
/// host bus during the HOB producer phase.
/// 
typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_RESOURCE_DESCRIPTOR.
  ///
  EFI_HOB_GENERIC_HEADER      Header;
  ///
  /// A GUID representing the owner of the resource. This GUID is used by HOB
  /// consumer phase components to correlate device ownership of a resource.
  ///
  EFI_GUID                    Owner;
  ///
  /// The resource type enumeration as defined by EFI_RESOURCE_TYPE.
  ///
  EFI_RESOURCE_TYPE           ResourceType;
  ///
  /// Resource attributes as defined by EFI_RESOURCE_ATTRIBUTE_TYPE.
  ///
  EFI_RESOURCE_ATTRIBUTE_TYPE ResourceAttribute;
  ///
  /// The physical start address of the resource region. 
  ///
  EFI_PHYSICAL_ADDRESS        PhysicalStart;
  ///
  /// The number of bytes of the resource region.
  ///
  UINT64                      ResourceLength;
} EFI_HOB_RESOURCE_DESCRIPTOR;

///
/// Allows writers of executable content in the HOB producer phase to 
/// maintain and manage HOBs with specific GUID.
/// 
typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_GUID_EXTENSION.
  ///
  EFI_HOB_GENERIC_HEADER      Header;
  ///
  /// A GUID that defines the contents of this HOB.
  ///
  EFI_GUID                    Name;
  //
  // Guid specific data goes here
  //
} EFI_HOB_GUID_TYPE;

///
/// Union of all the possible HOB Types.
///
typedef union {
  EFI_HOB_GENERIC_HEADER              *Header;
  EFI_HOB_MEMORY_ALLOCATION           *MemoryAllocation;
  EFI_HOB_RESOURCE_DESCRIPTOR         *ResourceDescriptor;
  EFI_HOB_GUID_TYPE                   *Guid;
  UINT8                               *Raw;
} EFI_PEI_HOB_POINTERS;


/**
  Returns the type of a HOB.

  This macro returns the HobType field from the HOB header for the 
  HOB specified by HobStart.

  @param  HobStart   A pointer to a HOB.
  
  @return HobType.
  
**/
#define GET_HOB_TYPE(HobStart) \
  ((*(EFI_HOB_GENERIC_HEADER **)&(HobStart))->HobType)

/**
  Returns the length, in bytes, of a HOB.

  This macro returns the HobLength field from the HOB header for the 
  HOB specified by HobStart.

  @param  HobStart   A pointer to a HOB.

  @return HobLength.

**/
#define GET_HOB_LENGTH(HobStart) \
  ((*(EFI_HOB_GENERIC_HEADER **)&(HobStart))->HobLength)

/**
  Returns a pointer to the next HOB in the HOB list.

  This macro returns a pointer to HOB that follows the 
  HOB specified by HobStart in the HOB List.

  @param  HobStart   A pointer to a HOB.

  @return A pointer to the next HOB in the HOB list.

**/
#define GET_NEXT_HOB(HobStart) \
  (VOID *)(*(UINT8 **)&(HobStart) + GET_HOB_LENGTH (HobStart))

/**
  Determines if a HOB is the last HOB in the HOB list.
  
  This macro determine if the HOB specified by HobStart is the 
  last HOB in the HOB list.  If HobStart is last HOB in the HOB list, 
  then TRUE is returned.  Otherwise, FALSE is returned.

  @param  HobStart   A pointer to a HOB.

  @retval TRUE       The HOB specified by HobStart is the last HOB in the HOB list.
  @retval FALSE      The HOB specified by HobStart is not the last HOB in the HOB list.

**/
#define END_OF_HOB_LIST(HobStart)  (GET_HOB_TYPE (HobStart) == (UINT16)EFI_HOB_TYPE_END_OF_HOB_LIST)

/**
  Returns a pointer to data buffer from a HOB of type EFI_HOB_TYPE_GUID_EXTENSION.

  This macro returns a pointer to the data buffer in a HOB specified by HobStart.
  HobStart is assumed to be a HOB of type EFI_HOB_TYPE_GUID_EXTENSION.   

  @param   GuidHob   A pointer to a HOB.

  @return  A pointer to the data buffer in a HOB.
  
**/
#define GET_GUID_HOB_DATA(HobStart) \
  (VOID *)(*(UINT8 **)&(HobStart) + sizeof (EFI_HOB_GUID_TYPE))

/**
  Returns the size of the data buffer from a HOB of type EFI_HOB_TYPE_GUID_EXTENSION.

  This macro returns the size, in bytes, of the data buffer in a HOB specified by HobStart.
  HobStart is assumed to be a HOB of type EFI_HOB_TYPE_GUID_EXTENSION.

  @param   GuidHob   A pointer to a HOB.

  @return  The size of the data buffer.
**/
#define GET_GUID_HOB_DATA_SIZE(HobStart) \
  (UINT16)(GET_HOB_LENGTH (HobStart) - sizeof (EFI_HOB_GUID_TYPE))


/**

 FSP specific GUID HOB definitions
 
 **/
#define FSP_INFO_HEADER_GUID \
  { \
  0x912740BE, 0x2284, 0x4734, {0xB9, 0x71, 0x84, 0xB0, 0x27, 0x35, 0x3F, 0x0C} \
  }

#define FSP_NON_VOLATILE_STORAGE_HOB_GUID \
  { \
  0x721acf02, 0x4d77, 0x4c2a, { 0xb3, 0xdc, 0x27, 0xb, 0x7b, 0xa9, 0xe4, 0xb0 } \
  }
  
#define FSP_BOOTLOADER_TEMPORARY_MEMORY_HOB_GUID \
  { \
  0xbbcff46c, 0xc8d3, 0x4113, { 0x89, 0x85, 0xb9, 0xd4, 0xf3, 0xb3, 0xf6, 0x4e } \
  }
  
#define FSP_HOB_RESOURCE_OWNER_FSP_GUID \
  { \
  0x69a79759, 0x1373, 0x4367, { 0xa6, 0xc4, 0xc7, 0xf5, 0x9e, 0xfd, 0x98, 0x6e } \
  }

#define FSP_HOB_RESOURCE_OWNER_TSEG_GUID \
  { \
  0xd038747c, 0xd00c, 0x4980, { 0xb3, 0x19, 0x49, 0x01, 0x99, 0xa4, 0x7d, 0x55 } \
  }

#define FSP_HOB_RESOURCE_OWNER_GRAPHICS_GUID \
  { \
  0x9c7c3aa7, 0x5332, 0x4917, { 0x82, 0xb9, 0x56, 0xa5, 0xf3, 0xe6, 0x2a, 0x07 } \
  }

#endif 
