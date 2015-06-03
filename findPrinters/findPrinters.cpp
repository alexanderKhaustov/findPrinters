// findPrinters.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <winspool.h>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
using namespace std;

vector<wstring> findPrinterNamesViaStruct1(DWORD type, LPTSTR Name);
vector<pair<wstring, wstring>> findPrinterNamesViaStruct2(DWORD type, LPTSTR Name);

extern void printIPAddress(const wstring &serverName);

int _tmain()
{
    setlocale(LC_ALL, "Russian");

    auto printProviders(findPrinterNamesViaStruct1(PRINTER_ENUM_NAME, NULL));
    for (auto pp : printProviders)
    {
        wcout << pp << "---------------" << endl;

        auto domains(findPrinterNamesViaStruct1(PRINTER_ENUM_NAME, (LPTSTR)pp.c_str()));
        for (auto d : domains)
        {
            if (d.find(L'!') == string::npos)
            {
                wcout << d << endl;
                continue;
            }

            auto printersAndServers(findPrinterNamesViaStruct1(PRINTER_ENUM_NAME, (LPTSTR)d.c_str()));
            for (auto pas : printersAndServers)
            {
                if (pas.find(L'!') == string::npos)
                {
                    wcout << pas << endl;
                    continue;
                }

                auto srvName = pas.substr(pas.find_last_of(L'!') + 3);
                wcout << "Server: " << srvName << endl;
                auto sharedPrinters(findPrinterNamesViaStruct1(PRINTER_ENUM_NAME, (LPTSTR)pas.c_str()));
                for (auto sp : sharedPrinters)
                {
                    wcout << sp << " : "; 
                    printIPAddress(srvName);
                    wcout << endl;
                }
            }
        }

//           auto remoteShared(findPrinterNamesViaStruct1(PRINTER_ENUM_NAME, /*L"Службы печати LanMan!WORKGROUP"*/NULL/*(LPTSTR)s.c_str()*/));
//         for (auto s : remoteShared)
//         {
//             wcout << s << endl;
// //            wcout << s.first << ":" << s.second << endl;
// //            getIPAddress(s);
//         }

        //         if (s.find(L"!!") == string::npos)
        //         {
        // //            wcout << s << endl;
        //             continue;
        //         }


//         auto remoteShared(findPrinterNamesViaStruct2(PRINTER_ENUM_NAME, (LPTSTR)s.c_str()));
//         for (auto s : remoteShared)
//         {
//             wcout << s.first << endl;
//             getIPAddress(s.second);
//         }
    }

    return 0;
}

vector<wstring> findPrinterNamesViaStruct1(DWORD type, LPTSTR Name)
{
    DWORD dwNeeded = 0;
    DWORD dwReturned = 0;
    DWORD dwBytes = 0;

    ::EnumPrinters( 
        type,	// types of printer objects to enumerate 
        Name,	 // name of printer object 
        1,		 // specifies type of printer info structure 
        NULL, // pointer to buffer to receive printer info structures 
        dwBytes,		// size, in bytes, of array 
        &dwNeeded,	// pointer to variable with no. of bytes copied (or required) 
        &dwReturned 	// pointer to variable with no. of printer info. structures copied 
        );
    vector<wstring> res;
    if (dwNeeded != 0)
    { 
        vector<BYTE> vBuf(dwNeeded);
        BYTE *pPrInfo = &vBuf[0];
        dwBytes  = dwNeeded;
        ::EnumPrinters( 
            type,	// types of printer objects to enumerate 
            Name,	// name of printer object 
            1,		 // specifies type of printer info structure 
            pPrInfo,	// pointer to buffer to receive printer info structures 
            dwBytes,	// size, in bytes, of array 
            &dwNeeded,	// pointer to variable with no. of bytes copied (or required) 
            &dwReturned 	// pointer to variable with no. of printer info. structures copied 
            );
        for (int i = 0; i < (int)dwReturned; i++)
            res.push_back(((PRINTER_INFO_1*)pPrInfo)[i].pName);
    }
    return res;
}

vector<pair<wstring, wstring>> findPrinterNamesViaStruct2(DWORD type, LPTSTR Name)
{
    DWORD dwNeeded = 0;
    DWORD dwReturned = 0;
    DWORD dwBytes = 0;

    ::EnumPrinters( 
        type,	// types of printer objects to enumerate 
        Name,	 // name of printer object 
        2,		 // specifies type of printer info structure 
        NULL, // pointer to buffer to receive printer info structures 
        dwBytes,		// size, in bytes, of array 
        &dwNeeded,	// pointer to variable with no. of bytes copied (or required) 
        &dwReturned 	// pointer to variable with no. of printer info. structures copied 
        );
    vector<pair<wstring, wstring>> res;
    if (dwNeeded != 0)
    { 
        vector<BYTE> vBuf(dwNeeded);
        BYTE *pPrInfo = &vBuf[0];
        dwBytes  = dwNeeded;
        ::EnumPrinters( 
            type,	// types of printer objects to enumerate 
            Name,	// name of printer object 
            2,		 // specifies type of printer info structure 
            pPrInfo,	// pointer to buffer to receive printer info structures 
            dwBytes,	// size, in bytes, of array 
            &dwNeeded,	// pointer to variable with no. of bytes copied (or required) 
            &dwReturned 	// pointer to variable with no. of printer info. structures copied 
            );
        for (int i = 0; i < (int)dwReturned; i++)
        {
            PRINTER_INFO_2 &prInfo =((PRINTER_INFO_2*)pPrInfo)[i]; 
            res.push_back(make_pair(prInfo.pPrinterName, prInfo.pServerName));
        }
    }
    return res;
}