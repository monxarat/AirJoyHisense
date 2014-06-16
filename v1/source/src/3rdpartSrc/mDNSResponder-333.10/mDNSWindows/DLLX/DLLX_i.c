

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Mar 28 11:23:17 2013
 */
/* Compiler settings for DLLX.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ITXTRecord,0x8FA0889C,0x5973,0x4FC9,0x97,0x0B,0xEC,0x15,0xC9,0x25,0xD0,0xCE);


MIDL_DEFINE_GUID(IID, IID_IDNSSDRecord,0x9CE603A0,0x3365,0x4DA0,0x86,0xD1,0x3F,0x78,0x0E,0xCB,0xA1,0x10);


MIDL_DEFINE_GUID(IID, IID_IDNSSDEventManager,0x7FD72324,0x63E1,0x45AD,0xB3,0x37,0x4D,0x52,0x5B,0xD9,0x8D,0xAD);


MIDL_DEFINE_GUID(IID, IID_IDNSSDService,0x29DE265F,0x8402,0x474F,0x83,0x3A,0xD4,0x65,0x3B,0x23,0x45,0x8F);


MIDL_DEFINE_GUID(IID, LIBID_Bonjour,0x18FBED6D,0xF2B7,0x4EC8,0xA4,0xA4,0x46,0x28,0x2E,0x63,0x53,0x08);


MIDL_DEFINE_GUID(IID, DIID__IDNSSDEvents,0x21AE8D7F,0xD5FE,0x45cf,0xB6,0x32,0xCF,0xA2,0xC2,0xC6,0xB4,0x98);


MIDL_DEFINE_GUID(CLSID, CLSID_DNSSDService,0x24CD4DE9,0xFF84,0x4701,0x9D,0xC1,0x9B,0x69,0xE0,0xD1,0x09,0x0A);


MIDL_DEFINE_GUID(CLSID, CLSID_TXTRecord,0xAFEE063C,0x05BA,0x4248,0xA2,0x6E,0x16,0x84,0x77,0xF4,0x97,0x34);


MIDL_DEFINE_GUID(CLSID, CLSID_DNSSDRecord,0x5E93C5A9,0x7516,0x4259,0xA6,0x7B,0x41,0xA6,0x56,0xF6,0xE0,0x1C);


MIDL_DEFINE_GUID(CLSID, CLSID_DNSSDEventManager,0xBEEB932A,0x8D4A,0x4619,0xAE,0xFE,0xA8,0x36,0xF9,0x88,0xB2,0x21);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



