
Descriptor Tool, Version 2.4

The major changes to 2.4 are:

1) Fixed monitor enumerated values. Needed to have usage 1 display "Enum 0".
   Added REPEATING0 option to .UPG files to fix this.

2) Added a title bar to the Usage Page Dialog box.

3) Fixed incorrect Extended Usages during inital insertion.

4) Added concatonation of Ordinal Instance value to Usages in collection
   immediately following Ordinal declaration.

5) Fixed display of Extended Usages when No Data option is selected.

6) Added support for Vendor Defined Usage Tables.


The major changes to Version 2.3 are:

1) added sizable window


The major changes to 2.0 are:

1) Incorporated the HID syntax checker from HIDview.

2) The Usage Pages are all ASCII files that can be modified by the user.

3) Extended Usages are supported.

4) Added cut and paste.

5) Report Descriptors files can be saved as .HID, .TXT, .INC, and .H. Only .HID can be read. 

6) Included examples from the HID spec. and the Usage Table document.

7) Fixed numerous bugs.



/************************************************************************
Instructions:

DT.INI must reside in the same directory as DT.EXE.

Usage pages can be added or modified. To add a Usage Page create a text
.UPG file as described below then add an Usage Page (UPn) entry to DT.INI.

NOTE: When saving a file in a format other than .HID be sure to
change the extension otherwise the .HID file will be overwritten.

****************************     WARNING      ***************************
The usages pages (.UPG files) from the 9.0 Release Candidate USB HID Usage
Tables document are included here. These pages are included for convienence
and may change in the final release of the document.
****************************     WARNING      ***************************


/************************************************************************
HID Spec issues:

1) Vendor defined Collection IDs are not supported by DT.

2) Extended Usages are 32-bit usages where the high order 16-bits are the
Usage Page ID and the low order 16-bits are the Usage ID. These are useful
when usages from different Usage Pages are required for the same Main Item.
Memphis supports extended usages. Extended Usages are not defined in the
HID 1.0 Specification.

3) Extended usage values for Usage Minimum and Maximum (32 bit parameters)
are also supported by DT. Memphis supports this too.

4) Memphis has the following restrictions on delimiters:
 - Do not use delimiters for usages associated with top level (application)
       collections.
 - Only Usages are supported in delimiters. Usage Min, Usage Max, Designators
       and Strings are NOT allowed in a delimited set.
 - Delimiters are not allowed when defining usages associated with an Array.

5) The checker (Parse Descriptor) does not support:
 -- Push and Pop - False errors will be reported due to state variables that
are not saved and restored.

 -- Extended Usages - The checker will report extended usages as "undefined"
because it does not break the 32-bit value into Usage and Usage Page.

Report problems to: steve_mcgowan@ccm.jf.intel.com


/************************************************************************
File format definitions:

/************************************************************************
.INI format

[UsagePages]
Count=<total number of usage pages declared>
UP0=<Usage Page ID>,<Usage Page Name>,<.UPG File Name>
UP1=...
...
UP<Count - 1>=...

  UPn, where n increments from 0 to the number of pages declared - 1

Notes:
  The .INI file must be in the same directory as DT.EXE.

  The Usage Page ID is in decimal.

  The Usage Page Name must be identical to that found in the .UPG file.

  The .UPG File Name can include a path.

*************************************************************************
.UPG format

There are 2 types of .UPG files: NORMAL and REPEATING.

NORMAL .UPG files

<Usage Page Name>
<.UPG File Type Flag: NORMAL>
<Usage ID><tab ('\t')><Usage Name> or
; Comment if ';' is first character of a line (ignored) or
<Usage ID-Usage ID><tab><Reserved of reserved> (ignored)

	Notes:
	The Usage Page Name must be identical to the Usage Page Name referred
	to in the .INI file to be recognized as a .UPG file.

	No blank lines can be in the file.

    The Usage ID is in hexidecimal with NO "0x" notation preceeding it
	or "h" following it.

	There are three forms that can follow the File Type Flag:
		1) The Usage ID, followed by a tab character, followed by the
		Usage Name Spaces are allowed in Usage names.
		2) Any line with a semi-colon as the first character is
		considered a comment and ignored.
		3) Any line that has  "Reserved" of "reserved" as the
		Usage Name will be considered a comment and ignored.
		4) Comments are only allowed after the UPG File Type Flag.

  Example:

Generic Desktop
NORMAL
00	Undefined
01	Pointer
02	Mouse
03	Reserved
04	Joystick
05	Game Pad
06	Keyboard
07	Keypad
08-2F	Reserved
30	X
...
81	System Power
82	System Sleep
83-FFFF	Reserved


REPEATING .UPG files

<Usage Page Name>
<.UPG File Type Flag: REPEATING>
00<tab ('\t')><first Usage Name>
01<tab ('\t')><repeated Usage Name>
; Comment if ';' is first character of a line (ignored)

	Notes:
	The Usage Page Name must be identical to the Usage Page Name referred
	to in the .INI file to be recognized as a .UPG file.

	No blank lines can be in the file.

	Only 2 usages are declared. The Usage Name of the first entry (00)
	will be used for the first usage and the Usage Name of the second
	entry will be repeated followed by <space><Usage ID>.

	There are three forms that can follow the File Type Flag:
		1) The Usage ID, followed by a tab character, followed by the
		Usage Name Spaces are allowed in Usage names.
		2) Any line with a semi-colon as the first character is
		considered a comment and ignored.
		3) Any line that has  "Reserved" of "reserved" as the
		Usage Name will be considered a comment and ignored.
		4) Comments are only allowed after the UPG File Type Flag.

  Example:

Button
REPEATING
00	Unused
01	Button


  Results in the usages:

Unused
Button 1
Button 2
Button 3
...

