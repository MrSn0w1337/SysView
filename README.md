# SysView
An open source windows research tool based on a kernel component

![alt text](http://i.epvpimg.com/LvPDcab.jpg)

Feature Overview:
  - Enumeration of running processes with information (PID, Name, Path, Threads)
  - Enumeration of device drivers with information (Name, Path, Base, Size)
  - Enumeration of process modules with information (Name, Path, Base, Entry, Size)
  - Enumeration of process threads with information (TID, Entry, State, Module context)
  - Ability to kill and suspend threads & processes
  - Ability to blacklist processes by a given name
  - Threads which does not belong to a valid module are marked with a red forecolor
  - Suspended threads and processes are marked with a white forecolor and grey backcolor

Planned Features:
  - Live overview for all loaded modules with filter options
  - Ability to hide processes and to edit process information
  - Ability to sort list views by columns
  - Ability to apply filter options for process, thread, module and driver overview
