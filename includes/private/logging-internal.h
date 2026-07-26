#ifndef __PCEM_LOGGING_INTERNAL_H__
#define __PCEM_LOGGING_INTERNAL_H__


// Used to flush the log contents to disk.
void pclog_flush();
// Used to close the file handle and flush the last contents to disk. Only use it when closing the application
void pclog_end();

#endif /* __PCEM_LOGGING_INTERNAL_H__ */