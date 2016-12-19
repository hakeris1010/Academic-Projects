.data
CRLF db 0dh, 0ah
CRLFE db 0dh, 0ah, '$'

STDIN  equ 0    ; Standard input device
STDOUT equ 1    ; Standard output device
STDERR equ 2    ; Standard error device           
STDAUX equ 3    ; Standard auxiliary device       
STDPRN equ 4    ; Standard printer device         

_whiteSpaceChars  db ' ' ;',.-+*/:;?!\\&()^#@~$'
_whiteSpaceCharsLenght equ $ - _whiteSpaceChars

cmdLineEnd dw 0FFFEh

; Stringai
str_startopenfile db 0Dh, 0Ah, '--- Opening a file ---', 0Dh, 0Ah, '$'
str_startread db 0Dh, 0Ah, '--- Starting to read a file ---', 0Dh, 0Ah, '$'
str_nofile db 0Dh, 0Ah, 'Opa failo ner! Klaida : ', '$'
str_filend db 0Dh, 0Ah, 'Failo pabaiga!', 0Dh, 0Ah, '$'

; ReadFile paramai.
rf_Flags       db 0             ; Flagai. Bitai: 0: 1-egz 0-spausd, 1: fail.uzd, 2: sk.visa faila.
rf_FileName    dw ?             ; failo vardas, 0-terminuotas.
rf_ExamineProc dw ?             ; egzaminacijos funkcija

rf_Buffer      dw ?             ; buferio starto pointeris
rf_BufLen      dw ?             ; buferio ilgis
rf_FileHandle  dw ?             ; failo handlo patalpinimo vieta.

rf_LoopsToRead dw 0FFFEh

; Bukles kintamieji.
rf_bytes       dw 0     ; kiek baitu perskaitem pask. faile.
rf_loops       dw 0     ; kiek skaitymo ciklu perejom.
rf_filesread   dw 0     ; kiek failu perskaitem.

; Kita.
_decimalString db 5 dup(' '), '$'
_hexString     db 4 dup('0'), '$'

ofn_fileOpenCmd dw 0

.code
printNumberDecimal proc ; atspausdina skaiciu desimtainiu pavidalu. 
; Parametrai: ax - skaicius, bx - stringo buferio pradzia, jei 0 - i konsole.    
    mov di, bx      ; di rodys i stringa arba i 0
    mov cx, 10
    mov bx, 4       ; kur desim simbus.
	
	notzero:
        cmp ax, 0
        jnz _st1a
            mov dl, ' '
            jmp short _res1b
        _st1a:
            xor dx, dx
            div cx			; dalinam ax is cx (10), remainderis DX'e, rezultatas ax'e
            add dl, '0'
        _res1b:
        cmp di, 0
        jnz _1a
            mov byte ptr [bx + offset _decimalString], dl
            jmp short _res1a
        _1a:    ; Jei DI rodo i stringa.
            mov byte ptr [di + bx], dl
        _res1a:
        dec bx
        cmp bx, 0
	jnl notzero		; sukam cikla kol ax ne 0.
    
    cmp di, 0
    jnz _end1a      ; Jei 0, spausdinam i COUT'a.
        lea dx, _decimalString
        mov ah, 09h
        int 21h
    _end1a:
    ret
printNumberDecimal endp

; Paramas: cx - kiek baitu nuo sp.
printStack proc
    mov si, sp
    xor bx, bx
    _loopst2:
        mov dl, byte ptr ss:[si + bx]
        mov ah, 02h
        int 21h			; atspausdinam 1 simboli is steko.	
        inc bx
        cmp bx, cx
    jnz _loopst2
    ret
printStack endp

; Gaunam stringo ilgi (iki 'dl'). Paramai: 
; - bx : buferis.
; - dl : deliminatorius. 
; Grazinam ax.
getStringLenght proc
    mov ax, bx
    dec bx
    loopop1:
        inc bx
        cmp byte ptr [bx], dl
    jnz loopop1
    sub bx, ax
    mov ax, bx
    ret
getStringLenght endp

; Nustatom ar charas yra skirtukas. Paramas ir grazinama reiksme al'e.
isCharWhiteSpace proc
    xor bx, bx
    cmp al, 20h     ; tarpas
    jg lstart1
        mov al, 1   ; jei maziau, tarpas.
        ret
    lstart1:
        cmp byte ptr [bx + offset _whiteSpaceChars], al
        jnz n11     ; jei skirtukas, tarpas.
            mov al, 1
            ret
        n11:
        inc bx
        cmp bx, _whiteSpaceCharsLenght
    jl lstart1
    xor al, al
    ret
isCharWhiteSpace endp

; 2 stringu palyginimas (DS'e)
; Args:
; - si : 1 stringas
; - di : 2 stringas
; - cx : lyginamu baitu sk.
; Rezultas: ax
compareStrings proc
    _loopSt3:
        mov dl, byte ptr [si]
        cmp dl, byte ptr [di]
        jnz _end3
        inc si
        inc di
    loop _loopSt3
    mov ax, 0
    ret
    _end3:
    mov ax, 1
    ret
compareStrings endp

;========================================================================;
; Arg:
; - ax : segment. 0 - SS, 1 - CS, 2 - DS, 3 - ES.
; - di : start of the block
ignoreBytes proc
    _loopStart11:
        cmp ax, 0   ; 0 - SS
        jnz _1aa
            mov dl, ss:[di]
        _1aa:
        cmp ax, 1   ; 1 - CS
        jnz _1ab
            mov dl, cs:[di]
        _1ab:
        cmp ax, 2   ; 2 - DS
        jnz _1ac
            mov dl, ds:[di]
        _1ac:
        cmp ax, 3   ; 3 - ES
        jnz _1ad
            mov dl, es:[di]
        _1ad:
        
        cmp dl, 32  ; Compare with space. Ignore all not-ASCII's [-128 - 0]
        jg _endLoop11 
        
        inc di
    jno short _loopStart11
    _endLoop11:
    ret
ignoreBytes endp

;========================================================================;
; Command line arg getter. Args:
; bx - last arg end (when start, then 81h)
; si - bufer adres
; dx - bufer len, if 0, then no buffer.
; Result: ax - Lenght of arg (nonzero) if got arg, if end, then 0.
getCmdLineArgToBuffer proc
    mov cx, [cmdLineEnd]
    cmp bx, cx ; jei bx didesnis uz komandines eilutes pabaiga, griztam.
    jb _strt1
    mov ax, 0
    ret
    
    _strt1:
    mov ax, si  ; ax - buf. pradzia.
    add dx, si  ; dx - buf.pabaiga
    _loopst1:    
        cmp si, dx ; ar buferio pabaiga.
        je _loopend2
        
        mov cl, es:[bx]
        cmp cl, 0dh ; jei eil. pabaiga (CR)
        je _argLineEnd1
        
        cmp cl, ' '
        jg _1
        cmp ax, si ; ar jau perskaitem nors 1 simboli, ir dabar tarpas.
        je _2 ; jei dar nieko neidejom i buferi
        jne _loopend2 ; jei jau kazka perskaitem ir tarpas tai vadinasi argumento pabaiga.
        _1:
        ;not tarp
        mov byte ptr [si], cl
        inc si
        _2: ; all cases - to next position in cmd line
        inc bx
    jmp short _loopst1
    _argLineEnd1:
    mov word ptr [cmdLineEnd], bx
    mov byte ptr es:[bx], ' '
    _loopend2:
    sub ax, si
    neg ax ; AX'as - kiek perskaitem.
    inc bx
    ret
getCmdLineArgToBuffer endp

; Get arg lenght. Params:
; bx - last arg end (when start, then 81h)
; Result: ax - Lenght of arg (nonzero) if got arg, if end, then 0.
;         di - start of arg.
;         bx - end of arg.
getCmdLineArg proc
    mov ax, 0
    mov cx, [cmdLineEnd]
    cmp bx, cx ; jei bx didesnis uz komandines eilutes pabaiga, griztam.
    jae _loopEnd3
    
    _strt2:
        cmp byte ptr es:[bx], 0dh
        jz _argLineEnd2
    
        cmp byte ptr es:[bx], ' '
        jg _2a
        cmp ax, 0
        jz _res2b           ; still good symbols
        jmp short _loopEnd3       ; if had syms, end.
        
        _2a:                ; on good symbol.
        cmp ax, 0
        jnz _2b
            mov di, bx      ; ax=0, di - arg start.
        _2b:  ; ax != 0
        inc ax
        _res2b:
        inc bx
    jmp short _strt2
    _argLineEnd2:
    mov word ptr [cmdLineEnd], bx
    mov byte ptr es:[bx], ' '
    _loopEnd3:
    ret
getCmdLineArg endp

; Rasom i faila.
; Argsai:
; - ax : failo handlas
; - bx : buferis. Ilgis - pirmame zodyje.
; Grazina ax - jei klaida ne 0.
writeToFile proc
    ; Write 2 file or DeViCe
    mov cx, word ptr [bx] ; cx - ilgis, gautas is bx'o 1 zodzio.
    mov dx, bx            ; dx - buferis. Mes ji persiuntem bx'e.
    add dx, 2             ; bufo duomenys - tik nuo 2 baito.
    mov bx, ax            ; bx - handlas. Gaunam is ax.
    mov ah, 40h           ; f-jos specifikatorius.
    int 21h
    ret
writeToFile endp

; Rasom i faila.
; Argsai:
; - ax : failo handlas
; - bx : buferis, 0-terminuotas
; Grazina ax - jei klaida ne 0.
writeStringToFile proc
    ; Write 2 file or DeViCe
    push ax
    push bx
    
    mov dl, 0
    call getStringLenght    ; buferis - bx'e. Res.Ilgis - ax'e
    mov cx, ax            ; Ilgis
    
    pop dx                ; dx - buferis. Mes ji persiuntem bx'e.
    pop bx                ; bx - handlas. Gaunam is ax.
    mov ah, 40h           ; f-jos specifikatorius.
    int 21h
    ret
writeStringToFile endp

; =============    File Reader from Name    =============
; Args:
; - ax : Filename start (0-term)
; - bx : buffer start (Standard, 1 word reserved for lenght)
; - cx : buffer capacity.
; - dx : buffer examine procedure.
readFileFromName proc
    ; Start procedures.
    mov word ptr [rf_FileName], ax    
    mov word ptr [rf_ExamineProc], dx
    
    ; Open File.
    mov dx, ax              ; put fname to dx
    mov al, 0               ; reading attrib
    mov ah, 3dh             ; open file id
    int 21h                 ; call this function - resulting handle on AX  
    jc _err12
    
    mov dx, [rf_ExamineProc]
    call readFile2
    ret
    _err12:
    call printFileError
    ret 
readFileFromName endp

; File Read from STDIN.
; Args:
; - ax : Loops to read (1 Loops - 255 bytes)
; - bx : buffer start (Standard, 1 word reserved for lenght)
; - cx : buffer capacity.
; - dx : buffer examine procedure.
readFromSTDIN proc
    mov word ptr [rf_LoopsToRead], ax
    mov ax, STDIN
    call readFile2
    mov word ptr [rf_LoopsToRead], 0FFFEh
    ret
readFromSTDIN endp

; Open FILENAME for attrib. Create if no file and AH is NonZero
; - dx : filename
; - al : attrib.
; - ah : bit 0 : Create File if Not Exist
;        bit 1 : Create File Always (And Purge!)
; Result : ax=0 - bad, ax=HANDLE - good.
openFileName proc
    ; Open File.
    mov ch, ah
    test ch, 02h        ; Test if purge (Create Always)
    jnz _createFile1
    ;mov dx,               ; put fname to dx
    ;mov al,                ; Open attrib
    mov ah, 3dh             ; open file id
    int 21h                 ; call this function - resulting handle on AX  
    jnc _end114
    
 ; If NeedToCreateFile.
    test ch, 01h            ; Test if create when not exist.
    jz _errTotal            ; If No creation (0), then print error - no file opened.
    
    _createFile1: 
    mov ah, 3Ch             ; Create file 
    mov cx, 6               ; File Attributes : none (Simple file)
    ;mov dx,                ; put fname to dx
    int 21h                 ; File Handle: AX
    ;jc _errTotal
    jnc _end114
    
 ; Open the created file again to get a handle.
    ;mov dx,               ; put fname to dx
    ;mov al,                ; Open attrib
    ;mov ah, 3dh             ; open file id
    ;int 21h                 ; call this function - resulting handle on AX  
    ;jnc _end114        
    
    _errTotal:
    call printFileError
    mov ax, 0
    _end114:
    ret
openFileName endp

; =============   File Reader from Handle   =============
; Args:
; - ax : Handle to a file.
; - bx : buffer start (Standard, 1 word reserved for lenght)
; - cx : buffer capacity.
; - dx : buffer examine procedure.
readFile2 proc
    ; Start procedures.
    mov word ptr [rf_FileHandle], ax
    mov word ptr [rf_Buffer], bx
    mov word ptr [rf_BufLen], cx
    mov word ptr [rf_ExamineProc], dx
    
    xor si, si ; Perskaityti Baitai
    xor di, di ; Ivykdyti Ciklai.
    
    ; Start looping through file.
    _rdLoopSt:
        mov ah, 3Fh                     ; skaitymo id
        mov bx, [rf_FileHandle]         ; bx'e - failo handlas
        mov cx, [rf_BufLen]             ; kiek baitu skaitysim 
        mov dx, [rf_Buffer]             ; buferio adresas
        add dx, 2                       ; Nuo 2 baito.
        int 21h                         ; AX'e - kiek perskaitem.
        jc _err11
        
        cmp ax, 0       ; Jei perskaityta 0, tai failas baigesi.
        je _rdLoopEnd

        mov bx, [rf_Buffer]             ; Buferio adresas i bx'a.
        mov word ptr [bx], ax           ; Kiek baitu patalpinta i bufa - pirmame zodyje.
        
        add si, ax      ; rf_bytes
        inc di          ; rf_loops
        
        push si
        push di   
        call [rf_ExamineProc]           ; Iskvieciam funka. Paramas: bx - bufer.startas.
        pop di
        pop si
        
        cmp di, word ptr [rf_LoopsToRead] ; Patikrinam ar dar nevirsijom nustatyto ciklu limito.
        jae _rdLoopEnd
    jmp short _rdLoopSt
    _rdLoopEnd:
    
    cmp word ptr [rf_FileHandle], 4
    jle _res2a                      ; if Handle is standard (0 - 4), don't close a file.
        ; Uzdarom faila.
        mov bx, [rf_FileHandle]     ; handlas.
        mov ah, 3Eh                 ; uzdarymo id
        int 21h
        jc _err11                   ; jei klaida (carry flag=1), tai baigiam.  
        
    _res2a:
    ; Ivykdom baigimo proceduras.
    mov word ptr [rf_bytes], si ; irasom duomenis.
    mov word ptr [rf_loops], di
    
    mov ax, [rf_filesread]
    inc ax
    mov word ptr [rf_filesread], ax
    
    ; Iskvieciam duom.spausdintoja.
    call printReadFileState
    ret
    _err11:
    call printFileError
    ret
readFile2 endp

; AX - error value.
printFileError proc
    mov cx, ax              ; klaidos inf.
    lea dx, str_nofile
    mov ah, 09h
    int 21h
    mov ax, cx
    mov bx, 0
    call printNumberDecimal
    ret
printFileError endp

; Call this after file read, to print temp. data about a file.
printReadFileState proc
    lea dx, CRLFE
    mov ah, 09h
    int 21h

    mov bx, 0
    mov ax, [rf_bytes]
    call printNumberDecimal
    
    mov bx, 0
    mov ax, [rf_loops]
    call printNumberDecimal
    
    mov bx, 0
    mov ax, [rf_filesread]
    call printNumberDecimal
    ret
printReadFileState endp

;========================================================================;