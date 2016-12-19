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
.code
include tools.asm

; =============    Examiner     =============
; Args:
; - bx : standard buffer start.
examineBuffer proc               ; egzaminuojam buferi - randam kiek zodziu, did.raidziu ir t.t.
    mov si, word ptr [bx]        ; buferio persk. baitu kiekis.
    mov byte ptr [bx + si + 2], '$'
    
    mov ax, [outFileHand]
    ;lea bx, [buffer]
    call writeToFile
    
    ret
examineBuffer endp

translateBufferFromHex proc
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
                jmp _end41_endLoop
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

testerTrololo proc
        ; -------------- Test ---------------
        mov si, ax 
        mov byte ptr [si + offset argBuff + 2], '$'          ; bufo pabaiga.
    
        mov dx, offset argBuff + 2     ; atspausdinam: rasyk i konsole.
        mov ah, 09h
        int 21h    
        lea dx, endline                 
        mov ah, 09h
        int 21h    
        
        ; Test.
        mov byte ptr [si], 0
        
        push bx ; Bx'e - sekancio argo pradzia.
        ;lea ax, [argBuff+2]        ; ax : Filename start (0-term)
        mov ax, 1
        lea bx, fileReadBuff        ; bx : buffer start (Standard, 1 word reserved for lenght)
        mov cx, fileReadBuffLen     ; cx : buffer capacity.
        lea dx, examineBuffer       ; dx : buffer examine procedure.
        call readFromSTDIN
        pop bx
        
        ret
testerTrololo endp

; ==============  void main() ===============
start:
	MOV ax, @data        ; perkelti data i registra ax
	MOV ds, ax           ; perkelti ax (data) i data segmenta
    
    ; lea si, test1
    ; lea di, test2
    ; mov cx, testLen
    ; repe cmpsb
    ; jz _equal1
        ; mov dl, 'n' ; Not equal
        ; mov ah, 02h
        ; int 21h
        ; jmp short enddd1
    ; _equal1:
        ; mov dl, 'e'
        ; mov ah, 02h
        ; int 21h
    ; jmp short enddd1
    
    mov bx, 81h          ; bx - cmd. eil. pradzia.
    _argLoop:            ; Isparsinam visus argumentus.
        ; bx - last arg end.
        call getCmdLineArg      ; AX - argumento ilgis, BX - arg.pabaiga, DI - arg. startas.
        cmp ax, 0
        jnz _next0
        jmp _loopend1
        
        _next0:        
    ; Pagalba.
        ;pop di
        push di
        lea si, arg_help
        mov cx, 3
        repe cmpsb      ; DS:[SI] - string 1, ES:[DI] - string 2.
        jnz _next1
            lea dx, help
            mov ah, 09h
            int 21h
            jmp programEnd
            
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
        
        ; lea si, arg_input
        ; mov cx, 3
        ; repe cmpsb      ; DS:[SI] - string 1, ES:[DI] - string 2.
        ; jnz _next2
            ; mov cl, [argFlags]
            ; test cl, 01h    ; 0000 0001  - jau buvo inputas.
            ; jz _res5a       ; jei dar nebuvo, tesiam
                ; lea dx, str_BadFormat
                ; mov ah, 09h
                ; int 21h
                ; jmp _loopend1
            ; _res5a:
            ; lea si, argBuff+2
            ; mov dx, argBuffLen
            ; call getCmdLineArgToBuffer      ; Get Filename1 - uses BX
            ; cmp ax, 0
            ; jnz _122
            ; jmp _loopend1
            ; _122:
            
            ; mov si, ax
            ; mov byte ptr [si + argBuff+2], 0
            
            ; ; Open File.
            ; lea dx, [argBuff+2]     ; put fname to dx
            ; mov al, 0               ; reading attrib
            ; mov ah, 3dh             ; open file id
            ; int 21h                 ; call this function - resulting handle on AX  
            ; jnc _openGood1
                ; lea dx, str_noFile
                ; mov ah, 09h
                ; int 21h
                
                ; mov word ptr [inFileHand], STDIN
                ; jmp short _resum1
            ; _openGood1:
                ; mov word ptr [inFileHand], ax
            ; _resum1:
            ; or cl, 01h      ; nustatom kad inputas jau buvo.
            ; mov byte ptr [argFlags], cl
            
            ; jmp short _next3
            
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
        
        ; lea si, arg_output
        ; mov cx, 3
        ; repe cmpsb      ; DS:[SI] - string 1, ES:[DI] - string 2.
        ; jnz _next3
            ; mov cl, [argFlags]
            ; test cl, 02h    ; 0000 0010  - jau buvo outputas.
            ; jz _res5b       ; jei dar nebuvo, tesiam
                ; lea dx, str_BadFormat
                ; mov ah, 09h
                ; int 21h
                ; jmp short _loopend1
            ; _res5b:
            ; lea si, argBuff+2
            ; mov dx, argBuffLen
            ; call getCmdLineArgToBuffer      ; Get Filename1 - uses BX
            ; cmp ax, 0
            ; jz _loopend1
            
            ; mov si, ax
            ; mov byte ptr [si + argBuff+2], 0
            
            ; ; Open File.
            ; lea dx, [argBuff+2]     ; put fname to dx
            ; mov ax, 0101h           ; writing attrib, and create if no.
            ; call openFileName
            ; cmp ax, 0
            ; jnz _openGood2
                ; lea dx, str_noFile
                ; mov ah, 09h
                ; int 21h
            
                ; mov word ptr [outFileHand], STDOUT
                ; jmp short _resum2
            ; _openGood2:
                ; mov word ptr [outFileHand], ax
            ; _resum2:
            ; or cl, 02h      ; nustatom kad outputas jau buvo.
            ; mov byte ptr [argFlags], cl
            ; ; jmp short _next3
            
        _next3:
            mov cl, [argFlags]
            cmp cl, 03h     ; was input and output
            jz _loopend1
        
    jmp _argLoop
    
    _loopend1:
        cmp word ptr [inFileHand], 4
        ja _res6a
            mov word ptr [rf_LoopsToRead], 2
        _res6a:
        mov ax, [inFileHand]        ; Handle for reading.
        lea bx, fileReadBuff        ; bx : buffer start (Standard, 1 word reserved for lenght)
        mov cx, fileReadBuffLen     ; cx : buffer capacity.
        lea dx, examineBuffer       ; dx : buffer examine procedure.
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


;==================================================================
; ; TEST Stuff.

; mov word ptr [argBuff], ax
        
        ; ; Tikrinam argo reiksmes.
        ; mov al, [argFlags]
        ; test al, 01h    ; formatas
        ; jz _value
        ; ; Formato parametrai
        ; ; Pagalba            
            ; lea si, arg_help
            ; ; lea di, argstart
            ; mov cx, 3
            ; repe cmpsb
            
            ; jnz _next1
                ; lea dx, help
                ; mov ah, 09h
                ; int 21h
                ; jmp short _loopend1
            
            ; _next1:
        ; ; Failu grandine
            ; lea si, arg_fileset
            ; ; lea di, argstart
            ; mov cx, 3
            ; repe cmpsb
            
            ; jnz _next2
                ; mov al, 02h     ; 0000 0010  - tikimes 1 failu grandines paramo.
                ; mov byte ptr [argFlags], al
                ; jmp short _value
                
        ; ; Default'as
            ; _next2:
            ; ; mov bx, [argBuff]
            ; ; mov byte ptr [bx + offset argBuff + 2], '$'
            
            ; ; lea dx, argBuff + 2
            ; ; mov ah, 09h
            ; ; int 21h
            ; lea dx, str_BadFormat
            ; mov ah, 09h
            ; int 21h
            ; jmp short _loopend1

;==================================================================

