; Programa: Nr. 3
; Uzduoties salyga: 8086 DaL. DiSaSeMbLeRiS 
; Atliko: Kestutis Dirma
; Versija: 0.3pre

; 0.3pre:
; - Geras argumentu parsinimo mechanizmas
; - Failo atidarymo/rasymo biblioteka
;   + Funkcija viso failo skaitymui buferiais ir ju apdorojimui, puikiai veikianti.

.model small
.stack 100h
	 
.data    
	help db 'Programa isveda failo statistinius duomenis.', 0dh, 0ah, 'Iveskite failu vardus kaip parametrus, pvz. uzd2.exe file1 file2', 0dh, 0ah, '$'
	badparams_std db 'Nera arba blogi paramai, rasykit duomenis i konsole.', 0Dh, 0Ah, '$'
    endline db 0Dh, 0Ah, '$'
    
    str_BadFormat db ' : Blogas formato specifikatorius', 0dh, 0ah, '$'
    str_BadValue  db ' : Bloga reiksme', 0dh, 0ah, '$'
    str_BadSymsInHexBuffer db 'Blogi simboliai sesioliktaineje sistemoje.', 0dh, 0ah, '$'
    
    rezultString db 0Dh, 0Ah, ' Simboliu: '
    rezultsimb   db 6 dup(32), ' Zodziu: '
    rezultzodz   db 6 dup(32), ' Didz. raidziu: '
    rezultdidrai db 6 dup(32), ' Maz. raidziu: '
    rezultmazrai db 6 dup(32), 'Nyaa~', '$'
    rezultStringLenght equ $ - rezultString - 1

    ; statiniai kintamieji saugoti uzduoties busenai.
	loops dw 0
	bytes dw 0
    
	; ---
    
    argBuff db 254 dup(0), '$'       ; failo vardo saugojimo buferis. (MAX_PATH=260)
    argBuffLen equ 252
    resultFileName db 254 dup(0), '$' ; kiekvieno failo rezultatu failvardis.
    
	inFileHand dw STDIN
    outFileHand dw STDOUT
    defOutFile db 'a.asm', 0
	
    fileReadBuff db 0, 0, 255 dup (0), '$' ; failo skaitymo duomenu bufas. Dydis - 255, 1 baitas - kiek perskaitem.
    fileReadBuffLen equ 255
    
    ; Argumentai
    arg_help db '/h '        ; print help message
    arg_input db '/i '       ; input file
    arg_output db '/o '      ; output file.
    
    ; Specifiniai bukles kintamieji, naudojami funkcijose ir statistikoje.
    statflags dw 0000h ; flagai, nurodantys kokioje busenoje baigem bloka (buferi)
    argsparsed dw 0  ; kiek argumentu apdorojom
    rezfileflag db 0
    
    argFlags db 00h     ; Argument flags: 
                        ; bit 0 : expecting format arg.
                        ; bit 1 : expecting value arg.
    argFlagTemp     db 0
    fileOpenCmdWord dw 0
    fileHandTemp    dw 0

;= = = =  DisAssembler State Variables  = = = =
asm_BuffLen         dw 0

asm_CommandString     db 80 dup(32), '$'
asm_CommandStringLen  equ $-asm_CommandString

asm_TempOperString    db 100 dup(32), '$'
asm_TempOperStringLen equ $-asm_TempOperString

asm_PrefixFlags     db 0    ; Flagas nurodantis kokie prefiksai yra nustatyti siai komandai.
    ; Bit 0 : REP
    ; Bit 1 : REPNE
    ; Bit 2 : LOCK
    ; Bit 3 : Segment Override
    ; Bit 4-5: Segment (00=CS, 01=DS, 10=ES, 11=SS)
    
asm_CommandSettings  db 0
; Command settings:
; - Bit 0 : Operates on words (1), bytes (0)
; - Bit 1 : Immediate Present
; - Bit 2 : Immediate Lenght:
;   + 0 : 16-bit
;   + 1 : 8-bit, need to be Sign-eXtended to produce 16-bit. 
; - Bit 3 : Direction Bit is present 
; - Bit 4 : Direction Bit value
; - Bit 5 : Register-Immediate instruction. No MOD field in ModRM. Use REG field as a reference.
; - Bit 6 : Extended opcode instruction. REG field is an extension. RM/Memory instructions.
; - Bit 7 : In ModRM, REG field is Replaced by SREG field.
asm_CommandSettings2 db 0
; Command Settings 2:
; - Bit 0 : Expecting Displacement (1) / Data (0)
; - Bit 1 : Expecting Disp/Data byte 0 or byte 1


    
; Current command state.
; String Pointers
asm_cmd_Prefix_StrPtr     dw 0
asm_cmd_OpCode_StrPtr     dw 0
asm_cmd_Operand1_StrPtr   dw 0
asm_cmd_Operand2_StrPtr   dw 0

; CommandSpecifiers
asm_curr_OpCode         db 0
asm_curr_ModRegRM       db 0
asm_curr_DataWord1      dw 0
asm_curr_DataWord2      dw 0

; Current TableState
asm_curTable_Start   dw 0
asm_curTable_Entries dw 0
asm_curTable_Width   dw 0
    
; Current BuffState
; Should be PUT INTO 1 BYTE. Now like this for easiness.
asm_onOpCode        db 0
asm_onModRM         db 0
asm_onData          db 0
asm_onCommandEnd    db 0

; Buffer End State:
asm_BufferEndState  db 0 
    ; 0 - Not on command
    ; 1 - on Prefix
    ; 2 - on OpCode
    ; 3 - on ModRM
    ; 4-7 - on Data 1-4

;= = = = = = = = = = =  = = = = = = = = = = = =
include opcode.asm
include tools.asm

.code
; =============     Real Deal     =============
; Table Param Setter
; - si : table start
; Result: cx: table end, si: first entry start, special asm_ vars set.
setTableParams proc
    mov word ptr [asm_curTable_Start], si

    mov ax, word ptr [si]   ; Entries in table
    mov word ptr [asm_curTable_Entries], ax
    
    mov cx, word ptr [si+2] ; Entry Lenght (Table Width)
    mov word ptr [asm_curTable_Width], cx
    
    mul cx         ; multiply ax(entries) * cx(lenght) to get the size -> result in AX
    add ax, si     ; Add the start of table.
    add ax, 4
    mov cx, ax     ; CX - the end of the table.
    
    add si, 4      ; SI - the beginning of the first entry.   
    
    ret
setTableParams endp

; Get the matching table entry start if matched with the first byte.
; - SI : table start 
; - BL : byte which we're matching to.
; Return: Found:     AL=1, SI : matched entry start
;         Not Found: AL=0
;         proper _asm variables set.
matchTableEntry proc
    call setTableParams   
    ; SI : entry beg, CX : table end.
    
    _TablLoopStrt:
        cmp si, cx
        jae _TablLoopEnd             ; If SI>=CX (Table end r34ched), end loop.
    
        cmp bl, byte ptr [si]
        jnz _resTablLoop             ; If not equal, go to next entry.
            mov al, 1
            ret         ; GOOD! We've found the match!
            
        _resTablLoop:
        add si, word ptr [asm_curTable_Width] ; Add an entry lenght to SI, to move to new entry.
        
    jmp short _TablLoopStrt    
    _TablLoopEnd:
    xor al, al          ; BAD. No match found.
    ret
matchTableEntry endp

; Prefix checker
; - [asm_curr_OpCode] : our full opc byte
; - result: asm_PrefixFlags byte set (if prefix)
;           al=0 (not prefix)
checkPrefix proc
    lea si, i_op_PREF_TABLE
    mov bl, byte ptr [asm_curr_OpCode]
    call matchTableEntry
    
    cmp al, 0       ; Check if found.
    jz _FuncEnd0    ; AL=0, not found, return.
    ; Not zero = PREFIX!
        mov cx, si
        add cx, 2
        mov word ptr [asm_cmd_Prefix_StrPtr], cx   ; String mnemonic of the prefix.
        
        mov bh, byte ptr [si+1]             ; Check the properties: Bit 0 is Segm.Override
        and bh, 01h
        shl bh, 3                           ; 0000 1000  - Segment Override flag on asm_Prefix
        mov byte ptr [asm_PrefixFlags], bh   ; set this prefix flag.
        ret ; AX - NonZero!
    
    _FuncEnd0:
    ret
checkPrefix endp

; Function for inspecting opcode of OpCode+R type:
; This type usually used for Immediate instructions.
; Opcode structure is like this:
; Bit 0-2 : REG field.
; Bit 3   : W bit
; Bit 4-7 : Opcode.
; Params:
; - [asm_curr_OpCode] : our full opc byte
; Result: asm_CommandSettings flags set appropprietaly
;           asm_curr_ModRegRM byte set appropprietaly 
inspectOpc_R proc
    mov ah, byte ptr [asm_curr_OpCode]
    mov al, ah
    
    and al, 08h     ; 0000 1000  - W flag.
    shr al, 1
    or byte ptr [asm_CommandSettings], al   ; Set immediate lenght
    shr al, 2
    or byte ptr [asm_CommandSettings], al   ; Set W flag on our settings.
    
    mov al, ah
    and al, 07h     ; 0000 0111  - Get REG value.
    shl al, 3
    or byte ptr [asm_curr_ModRegRM], al     ; Set our ModRM byte
    
    or byte ptr [asm_CommandSettings], 22h  ; 0010 0010 - Set that Immediate is present, and no MOD field in ModRM (Immediate instruction). 
    ; TODO: This should be replaced with more abstract things, coz there are instr's of this format with no immediates. (ex. PUSH reg)
    
    ; Now we move to ModRM, which is already set..
    mov byte ptr [asm_onModRM], 1
    mov al, 1
    ret
inspectOpc_R endp

; Function for inspecting Special OpCodes, Where W and D bits are used as part of an OpCode.
; Params:
; - [asm_curr_OpCode] : our full opc byte
; - SI : pointer to Start of matching table entry.
; Result: asm_CommandSettings flags set appropprietaly
;         al=0 if bad
inspectSpecialOpc proc
    mov ah, byte ptr [asm_curr_OpCode]  ; AH=Our Full Opcode.
    cmp byte ptr [si], ah       ; Check if our full opcode match with full table opcode.
    jnz _specOpcEnd_Bad
    mov dl, ah                  ; DL: Backup for modification
    
    ; TODO: Inspection using table, no direct references. 
    ; Now we check all special Opcodes manually.
    cmp byte ptr [i_op_MOV_rm_Sreg], ah
    jz _spNxt1
    cmp byte ptr [i_op_MOV_Sreg_rm], ah
    jnz _spNxt2
    
    _spNxt1: ; i_op_MOV_Sreg_rm
    or byte ptr [asm_CommandSettings], 89h  ; W=1, D pres., REG=SREG
    and dl, 02h         ; 0000 0010  - D bit
    or byte ptr [asm_CommandSettings], dl
    mov byte ptr [asm_onModRM], 1
    jmp short _specOpcEnd_Good
    
    _spNxt2:
    cmp byte ptr [i_op_MOV_rALX_moff], ah
    jnz _specOpcEnd_Bad
    
    and dl, 01h
    or byte ptr [asm_CommandSettings], dl   ; Set W flag on our settings.
    mov dl, al
    and dl, 02h     ; 0000 0010
    shl dl, 3       ; 0001 0000
    or dl, 28h      ; 0010 1000  - D present, Reg-Imm instr.
    or byte ptr [asm_CommandSettings], dl
    mov [asm_curr_ModRegRM], 0  ; 000 - Use AL or AX
    mov [asm_onModRM], 1
    
    _specOpcEnd_Good:
    mov al, 1
    ret
    _specOpcEnd_Bad:
    xor al, al
    ret
inspectSpecialOpc endp

; Params
; - al : byte.
; Result: specific asm_ variables set, 
;   changed the asm_onSomething vars,
;   and AL:
;   = 0 : UnRecognized byte.
;   = 1 : Prefix identified
;   = 2 : OpCode identified
checkOpcode proc    
    mov byte ptr [asm_curr_OpCode], al       ; AL - original opcode byte.
    
    call checkPrefix        ; Check if curr.byte is a prefix. 
    
    cmp al, 0
    jz _chkOpc  ; If al=0, Prefix wasn't found. Then we gotta check for opcode.
    jmp _funcEnd1_Pref
    _chkOpc:
    ; If AX=0, not prefix. Check if OpCode.
        mov bl, byte ptr [asm_curr_OpCode]
        and bl, 0FCh        ; Standard Opcode, 2 low bits null'd
        
        lea si, t_OPCODE_TABLE
        call matchTableEntry
        
        cmp al, 0   
        jz _funcEnd1_No     ; NOT FOUND. The opcode is UnRecognized.
        
        ; FOUND!!!        
        mov bh, byte ptr [si+1] ; Get the properties byte.
        
    ; START Testing Properties.
    ; If needs further inspection
        test bh, 01h            ; Check if Opcode+R
        jz _opNxt1
            call inspectOpc_R   ; Set fields for Opc+R opcode type. 
            cmp al, 0
            jnz _funcEnd1_OpCod ; If AL=1, Good opcode, if 0, bad.
            jmp short _funcEnd1_No
        _opNxt1: ; Standart opcode.
        
        test bh, 10h            ; Check if Special OpCode (Needs further inspection)
        jz _opNxt2
            call inspectSpecialOpc
            cmp al, 0
            jnz _funcEnd1_OpCod
            jmp short _funcEnd1_No
        _opNxt2: 
        
    ; Standard flags, no further inspection required.
    ; ModRM existence
        test bh, 02h            
        jz _opNxt3
            mov byte ptr [asm_onModRM], 1   ; Now gonna move to ModRM.  
        _opNxt3: 
    
    ; Extended opcode. Set flag if present.
        test bh, 08h            
        jz _opNxt4
            or byte ptr [asm_CommandSettings], 40h  ; Set extended flag.
        _opNxt4: 
        
    ; Process the D bit
        test bh, 20h            ; Check if D bit is not used
        jz _opNxt5
            and byte ptr [asm_CommandSettings], 0F7h    ; And with 1111 0111
            ; Check if our OpCode has this bit
            jmp short _opNxt7
        _opNxt5: 
    
        mov dl, byte ptr [asm_curr_OpCode]  ; Load the bit to DL
        and dl, 02h
        
        test bh, 04h            ; Check if D bit replaced by S (1)
        jz _opNxt6
            ; 1, S bit is present instead of D.
            or byte ptr [asm_CommandSettings], 02h  ; Imm. present.
            shl dl, 1                               ; Shift S bit in DL to appropriate position
            or byte ptr [asm_CommandSettings], dl   ; Save settings.
            jmp short _opNxt6
        _opNxt6: 
            ; 0, D bit is present.
            or byte ptr [asm_CommandSettings], 08h
            shl dl, 3
            or byte ptr [asm_CommandSettings], dl   ; Save settings.
        
        _opNxt7:
    ; Process the W bit
        ;test bh, 40h            ; Check if W bit is not used
        ; Always used, except the Special opcodes.
        mov dl, byte ptr [asm_curr_OpCode]  ; Load the bit to DL
        and dl, 01h
        or byte ptr [asm_CommandSettings], dl   ; Save settings. no shifting needed, bit 0 on both.
        
    _funcEnd1_OpCod:    ; Perform Ending tasks.
    add si, 2
    mov word ptr [asm_cmd_OpCode_StrPtr], si    ; The Mnemonic string is ready.
    
    mov byte ptr [asm_onOpCode], 0      ; No longer on OpCode.
    mov al, 2
    ret     ; OpCode ID'd
        
    _funcEnd1_Pref:
    mov al, 1
    ret
    _funcEnd1_No:
    xor al, al
    ret
checkOpcode endp

; Function for inspecting ModRM.
; Params:
; - al : Newly Got byte.
; - [asm_curr_ModRegRM] : our full ModRM byte, if certain flags are set will be inspected.
; Result: asm_CommandSettings flags set appropprietaly
;         asm_cmd_ vars set 
;         changed the asm_onSomething vars
;         al=0 if bad
inspectModRM proc
    ; - Bit 5 : Register-Immediate instruction. No MOD field in ModRM. Use REG field as 
    ; - Bit 6 : Extended opcode instruction. REG field is an extension. RM/Memory instructions.
    ; - Bit 7 : In ModRM, REG field is Replaced by SREG field.
    
;=====     Check all the special cases     =====;
    test byte ptr [asm_CommandSettings], 20h    ; Check Bit 5.
    jz _mrmNxt1
; DO this if Register-Immediate instruction. (Opcode was Opc+R type).
; - REG field specifies operand.
        mov ah, byte ptr [asm_curr_ModRegRM]
        shr ah, 3
        and ah, 07h     ; 0000 0111     - The REG Table.
        
        mov al, byte ptr [asm_CommandSettings]
        and al, 01h     ; 0000 0001  - Leave the W bit.
        shl al, 3       ; 0000 1000
        or ah, al       ; Get the value for table inspection. (W+REG)
        
        lea si, t_REG
        mov bl, ah
        call matchTableEntry
        
        cmp al, 0
        jz _mrmEndp_Bad
        ; If good:
        add si, 1
        mov word ptr [asm_cmd_Operand1_StrPtr], si  ; Our Operand1 : Simple register.
        
        mov byte ptr [asm_onData], 1
        ; Now we should be on Data byte, but ended up here coz the loop goes like this.
        ; So we gotta jump to InspectData, and then the Ret will return us directly to the MainLoop
        jmp inspectData
        
        ;jmp short _mrmEndp_Good
        
    _mrmNxt1:
    test byte ptr [asm_CommandSettings], 80h    ; Check Bit 7.
    jz _mrmNxt2
; Do this if REG field is Replaced by SREG field.
        
        
        jmp short _mrmEndp_Good
    
    _mrmNxt2:
    test byte ptr [asm_CommandSettings], 40h    ; Check Bit 6.
    jz _mrmNxt3
; Do this if Extended opcode instruction. 
; - REG field is an extension of an OpCode. We'll ignore it by now.
; - R/M field specifies Memory operand.
; - We expect Displacement and Data after this byte.
        mov ah, byte ptr [asm_curr_ModRegRM]
        and ah, 07h     ; 0000 0111     - The R/M Field.
        
        
        
        mov al, byte ptr [asm_CommandSettings]
        and al, 01h     ; 0000 0001  - Leave the W bit.
        shl al, 3       ; 0000 1000
        or ah, al       ; Get the value for table inspection. (W+REG)
        
        lea si, t_REG
        mov bl, ah
        call matchTableEntry
        
        cmp al, 0
        jz _mrmEndp_Bad
        ; If good:
        add si, 1
        mov word ptr [asm_cmd_Operand1_StrPtr], si  ; Our Operand1 : Simple register.
        
        mov byte ptr [asm_onData], 1
        ; Now we should be on Data byte, but ended up here coz the loop goes like this.
        ; So we gotta jump to InspectData, and then the Ret will return us directly to the MainLoop
        jmp inspectData
        
        jmp short _mrmEndp_Good
        
    _mrmNxt3:
;=====     Now check the standard case : Mod Reg R/M     =====;
    
    
    _mrmEndp_Good:
    mov byte ptr [asm_onModRM], 0   ; No longer on ModRM
    mov al, 1
    ret
    _mrmEndp_Bad:
    xor al, al
    ret
inspectModRM endp

; Params:
; - al : current byte (of expected data)
; Result: asm_cmd_ vars set 
;         changed the asm_onSomething vars
;         al=0 if bad
inspectData proc
    ret
inspectData endp

; =============     Examiner      =============
; Args:
; - bx : standard buffer start.
examineBuffer proc               ; egzaminuojam buferi - randam kiek zodziu, did.raidziu ir t.t.
    mov ax, word ptr [bx]   ; First word of Buffer : lenght.
    add ax, bx              ; Add Buffer start to it's lenght to get the end o'da buffah.
    add ax, 2               
    mov word ptr [asm_BuffLen], ax
    
    add bx, 2       ; BX - start o' buffer data.
    mov si, bx      ; SI - buffer crawler pointer.
    
    ; SET STATE VARS (asm_onPrefix, onOpcode...), according to backup'd state on asm_BufferEndState
    
    mov byte ptr [asm_onOpCode], 1
    
    _disAsmLoopStart:
        cmp si, word ptr [asm_BuffLen]
        jae _disAsmLoopEnd
        push si
        push bx
        
        _onOpcode: ; And on Prefix.
        cmp byte ptr [asm_onOpCode], 0
        jz _onModRM  
            ; Do stuff if on OpCode
            mov al, byte ptr [si]   ; AL - current byte on inspection.
            
            ; Param: AL.
            call checkOpcode    ; Return AL: 2-opc, 1-pref, 0-not id'd
            cmp al, 2
            jnz _res8
                mov ax, [outFileHand]
                mov bx, [asm_cmd_OpCode_StrPtr]
                call writeStringToFile
                
            _res8:   
        _onModRM:
        cmp byte ptr [asm_onModRM], 0
        jz _onData
            ; Do stuff if on ModRM
            
        _onData:
        cmp byte ptr [asm_onData], 0
        jz _asmLoopCont
            ; Do stuff if on Data, Many stuff to do here...
        
        _asmLoopCont:
        pop bx
        pop si
        
        inc si
        ; update other vars
    
    jmp short _disAsmLoopStart    
    
    _disAsmLoopEnd:     ; Do ending jobs: set buffer end state vars according to current state.
    
    
    ret
    
    _oldExamPrint1:
    mov si, word ptr [bx]        ; buferio persk. baitu kiekis.
    mov byte ptr [bx + si + 2], '$'
    
    mov ax, [outFileHand]
    ;lea bx, [buffer]
    call writeToFile
    
    ret
examineBuffer endp

; ============= - - - - - - - - - - - - - - - - - ============= ;
; =============    Helper and Start Functions     ============= ;
; Args:
; - bx : standard buffer start.  Buffer data is expected to be ONLY IN HEX, and no spaces or whatsoever.
translateBufferFromHex proc
    mov cx, [bx]        ; Buffer lenght - first word.
    add cx, bx          ; CX = Buffer End.
    lea si, [bx + 2]    ; Buffer data current position.
    mov di, si          ; Buffer converted data current pos.
    
    mov dh, 0           ; Flag : 0 - first nibble, 1 - second nibble.
    _lpStart:
        cmp si, cx
        ja _lpEnd
        cmp di, cx      ; Maybe unneeded, but for bug check.
        ja _lpEnd
        
        mov al, byte ptr [si]
        call translateHexToByte
        cmp ah, 0       ; If ah=0, hex symbol is invalid.
        jz _badEnd      ; The current nibble is in AL.
        
        cmp dh, 0
        jz _nekstLp
            shl dl, 4   ; If dh=1, this is the second nibble, and we're ready to make it a byte.
                        ; We shift the backup'd nibble by 4 to get it to high order.
            or al, dl   ; In AL we have the low order 4 bits, so we OR them to get the full byte in AL
            
            mov byte ptr [di], al   ; We put our byte to the converted buffer position Di.
            inc di
        _nekstLp:
        mov dl, al      ; Backup current nibble.
        xor dh, 01h     ; Switch dh's 1st byte.
        inc si
    jmp short _lpStart
    _badEnd:
    ;lea dx, str_BadSymsInHexBuffer
    ;mov ah, 09h
    ;int 21h                     ; Print that bad syms were found.
    _lpEnd:
    sub di, bx                  ; To get new lenght, we subtract DI (Curr.pos.) from BX (Buff.start),
    sub di, 2                   ; and then subtract 2, because the 1st word is lenght.
    mov word ptr [bx], di       ; The new BuffLen will be the converted one, from DI.
    
    ; In the end we call the Buffer Examiner.
    ; BX : buff. start.
    call examineBuffer
    
    ret
translateBufferFromHex endp
    
; ============= Argument executor =============
; Args:
; - bx : current position in arg buffer
; - si : (ES) - checking arg 1
; - di : (ES) - checking arg 2
; - cl : flag which needs to be checked and set.
; - ax : file opening cmdline
; - dx : pointer to FileHandle to write to.
checkAndExecuteArgument proc
    ;lea si, arg_input
    mov byte ptr [argFlagTemp], cl
    mov word ptr [fileOpenCmdWord], ax
    mov word ptr [fileHandTemp], dx
    
        mov cx, 3
        repe cmpsb      ; DS:[SI] - string 1, ES:[DI] - string 2.
        jnz _end41
            mov cl, [argFlags]
            test cl, byte ptr [argFlagTemp]    ; 0000 0001  - jau buvo inputas.
            jz _res5a       ; jei dar nebuvo, tesiam
                lea dx, str_BadFormat
                mov ah, 09h
                int 21h
                jmp short _end41_endLoop
            _res5a:
            lea si, argBuff+2
            mov dx, argBuffLen
            call getCmdLineArgToBuffer      ; Get Filename1 - uses BX
            cmp ax, 0
            jz _end41_endLoop
            
            mov si, ax
            mov byte ptr [si + argBuff+2], 0
            
            ; Open File.
            lea dx, [argBuff+2]       ; put fname to dx
            mov ax, [fileOpenCmdWord] ; file opening options
            call openFileName         ; call this function - resulting handle on AX  
            
            mov si, [fileHandTemp]          ; Failo handlo vieta atmintyje.
            cmp ax, 0
            jnz _openGood1
                lea dx, str_noFile
                mov ah, 09h
                int 21h
                
                mov word ptr [si], STDIN    ; SI rodo i failo handla i kuri rasysim.
                jmp short _resum1
            _openGood1:
                mov word ptr [si], ax
            _resum1:
            mov cl, [argFlags]
            or cl, byte ptr [argFlagTemp]      ; nustatom kad inputas jau buvo.
            mov byte ptr [argFlags], cl
            
            jmp short _end41
    _end41_endLoop:
    mov ax, 0
    ret
    _end41:
    mov ax, 1
    ret
checkAndExecuteArgument endp

; ==============  void main() ===============
start:
	MOV ax, @data        ; perkelti data i registra ax
	MOV ds, ax           ; perkelti ax (data) i data segmenta
    
    mov bx, 81h          ; bx - cmd. eil. pradzia.
    _argLoop:            ; Isparsinam visus argumentus.
        ; bx - last arg end.
        call getCmdLineArg      ; AX - argumento ilgis, BX - arg.pabaiga, DI - arg. startas.
        cmp ax, 0
        jz _loopend1
        
        _next0:        
    ; Pagalba.
        push di
        lea si, arg_help
        mov cx, 3
        repe cmpsb      ; DS:[SI] - string 1, ES:[DI] - string 2.
        jnz _next1
            lea dx, help
            mov ah, 09h
            int 21h
            jmp short programEnd
            
        _next1: 
    ; Inputas
        pop di
        push di
        
        lea si, arg_input
        mov cl, 01h
        mov ax, 0000h
        lea dx, inFileHand
        call checkAndExecuteArgument
        
        cmp ax, 0
        jz _loopend1
            
        _next2:
    ; Outputas
        pop di
        
        lea si, arg_output
        mov cl, 02h
        mov ax, 0201h
        lea dx, outFileHand
        call checkAndExecuteArgument
        
        cmp ax, 0
        jz _loopend1
    ; Pabaigos tikrinimas
        _next3:
        mov cl, [argFlags]
        cmp cl, 03h     ; was input and output
        jz _loopend1
        
    jmp _argLoop
    
    _loopend1:
        cmp word ptr [inFileHand], 4
        ja _res6b
            mov word ptr [rf_LoopsToRead], 1    ; If from STDIN, read 1 loop.
            lea dx, translateBufferFromHex
            jmp short _res6a
        _res6b:
        lea dx, examineBuffer       ; dx : buffer examine procedure.            
        _res6a:
        mov ax, [inFileHand]        ; Handle for reading.
        lea bx, fileReadBuff        ; bx : buffer start (Standard, 1 word reserved for lenght)
        mov cx, fileReadBuffLen     ; cx : buffer capacity.        
        call readFile2
        
        mov word ptr [rf_LoopsToRead], 0FFFEh
    
    ;jmp short enddd1
    
   
    programEnd:
    ; Jei reikia, uzdarom rezult. faila.
    mov bx, [outFileHand]
    cmp bx, 1
    jz enddd1  ; jeigu 1, tai stdout, nieko nedarom
        ; jeigu ne 1 (ne STDOUT), tai reikia uzdaryt.
        mov ah, 3Eh             
        int 21h             
        
    enddd1:
	mov 	al, 0 		; be klaidu = 0
	mov 	ah, 4ch 	; sustabdyti programa 
	int 	21h         ; 21h -  dos pertraukimmas
end start
