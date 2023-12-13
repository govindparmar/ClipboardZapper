# ClipboardZapper
Automatically censors certain phrases copied on the clipboard with a different character.
                                                                 
For example, if this program is configured to censor the phrase "SUPERSECRETPASSWORD123", and the user on the workstation copied the text "my password is SUPERSECRETPASSWORD123", when they pasted the content of the clipboard, it would be "my password is **********************"

This program's settings are stored in `HKEY_CURRENT_USER\Software\GovindParmar\ClipboardZapper`. 
The value `ZapChar` is a 32-bit `REG_DWORD` value whose upper and lower halves constitute the two wide characters to replace censored phrases with.
If only the low order bits of the `DWORD` are set, only they are used.
The value `ZapList` is a `REG_MULTI_SZ` value which is an array of null-terminated strings terminated by a final null character. 
