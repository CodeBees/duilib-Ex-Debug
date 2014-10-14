void Trace(char* fmt,... )
{
	char buf[1024];
	va_list args;
	va_start(args,fmt);
	vsprintf(buf,fmt,args);
	va_end(args);

#ifdef _UNICODE
	OutputDebugStringW(buf)
#else
	OutputDebugStringA(buf);
#endif
}
	
