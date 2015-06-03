#include <string>
using namespace std;

using namespace System;
using namespace System::Net;

void printIPAddress(const wstring &serverName)
{
    String ^name = gcnew String(serverName.c_str());
    auto addr = Dns::GetHostAddresses(name);
    Console::Write("{0}", addr);
}