; Programa: Nr. 1
; Uzduoties salyga: 
; Atliko: Kestutis Dirma
; Versija: 1.0 

; Viskas veikia kaip turi.

.model small
.stack 100h
	 
.data
	help db 'Programa isveda failo statistinius duomenis.', 0dh, 0ah, 'Iveskite failu vardus kaip parametrus, pvz. uzd2.exe file1 file2', 0dh, 0ah, '$'
	badparams_std db 'Nera arba blogi paramai, rasykit duomenis i konsole.', 0Dh, 0Ah, '$'
    nofile db 0Dh, 0Ah, 'Opa failo ner! Klaida : ', '$'
    filendstr db 0Dh, 0Ah, 'Failo pabaiga!', 0Dh, 0Ah, '$'
    endline db 0Dh, 0Ah, '$'
    
    rezultsimb   db ' Simboliu: ', '$'
    rezultzodz   db ' Zodziu: ', '$'
    rezultdidrai db ' Didz. raidziu: ', '$'
    rezultmazrai db ' Maz. raidziu: ', '$'
    
    ;debugs
    startopenfile db 0Dh, 0Ah, '--- Opening a file ---', 0Dh, 0Ah, '$'
    startread db 0Dh, 0Ah, '--- Starting to read a file ---', 0Dh, 0Ah, '$'

    ; statiniai kintamieji saugoti uzduoties busenai.
	simb   dw 0 
	zodz   dw 0
	mazrai dw 0
	didrai dw 0
    statflags db 04h ; flagai, nurodantys kokioje busenoje baigem bloka (buferi)
    filesread dw 0   ; kiek failu perskaitem.
	; ---
    
    filename db 254 dup(0), '$'      ; failo vardo saugojimo buferis. (MAX_PATH=260)
    
	filestart  dw ?
    filehandle dw ?
	
    fileReadBuff db 0, 0, 255 dup (0), '$' ; failo skaitymo duomenu bufas. Dydis - 255, 1 baitas - kiek perskaitem.
    fileReadBuffLen equ 255
.code
jmp start

; Functions

printNumberDecimal: ; atspausdina skaiciu desimtainiu pavidalu. Parametras: ax.
	mov cx, 10
    mov dx, 32
	push dx 	; i steka idedam tarpa - zenklas baigti traukti simbolius.
	
	notzero:
    xor dx, dx
	div cx			; dalinam ax is cx (10), remainderis dx'e, rezultatas ax'e
	push dx			; nupushinam remainderi i steka.
	cmp ax, 0
	jnz notzero		; sukam cikla kol ax ne 0.
	
	notend: 
	pop dx
    cmp dx, 32
    jz end1
    
    add dx, '0'     ; paruosiam spausdinimui.
    mov ah, 02h
	int 21h			; atspausdinam 1 simboli is steko.	
	jmp notend
    end1:
ret

examineBuffer:      ; egzaminuojam buferi - randam kiek zodziu, did.raidziu ir t.t.
    mov cl, [statflags]         ; cl'as - flagai. Dabar paimam busena is paskutinio bloko.
                                ; Bitai is desines:
                                ; 8 - didesnis uz 1
                                ; 7 - mazesnis uz 2
                                ; 6 - pask.simb. tarpas
                                ; 5 - simb. yra tarp 'A' ir 'Z'
                                ; 4 - simb. yra tarp 'a' ir 'z'
    ;mov ah, 09h
    ;lea dx, startopenfile
    ;int 21h
    
    mov si, offset fileReadBuff + 2      ; cia saugosim kur dabar esam buferyje.
    mov bx, word ptr [fileReadBuff]      ; kiek simboliu yra buferyje.  
    add bx, si                  ; buferio pabaigos pozicija (pridedam buferio pradz. adresa)
    
    loopbuf1:    
        mov dl, byte ptr [si]
        ;mov dl, '.'
        ;mov ah, 02h
        ;int 21h	
        
        and cl, 04h      ; 00000100 - tarpo informacija yra vienintelis paliekamas dalykas.
        
        ; Tikrinam ar mazoji raide.
        cmp dl, 'a'
        jnge la3
        cmp dl, 'z'
        jnle la3
        
        or cl, 10h    ; 00010000 - jis yra mazoji raide.
        
        mov ax, word ptr [mazrai] ; padidinam mazuju raidziu skaiciu.
        inc ax
        mov word ptr [mazrai], ax 
        la3:
        
        ; Tikrinam ar didzioji raide.
        cmp dl, 'A'
        jnge la6
        cmp dl, 'Z'
        jnle la6
        
        or cl, 08h    ; 00001000 - jis yra didzioji raide.
        
        mov ax, word ptr [didrai] ; padidinam didz.raidziu skaiciu.
        inc ax
        mov word ptr [didrai], ax 
        la6:
        
        ; Nustatom ar tarpas
        cmp dl, 20h     ; tarpas
        jg la7          ; Jei daugiau uz tarpa, sokam prie ne tarpu.
                        ; jei maziau arba lygu - tai whitespace char'ai (tarpai)
        or cl, 04h      ; 00000100 - tai  tarpas.
        jmp la8
        
        la7:  ; Jeigu ne tarpas.
        test cl, 04h    ; tikrinam ar pask. baitas buvo tarpas.
        jz la9          ; jeigu 0, tai buvo ne tarpas, ir dabar ne tarpas, tad sokam prie pabaigos.
                        ; jeigu 1, tai pask. baitas buvo tarpas, bet dabar ne, todel zodzio pradzia.
        mov ax, [zodz]  ; Ivykdom zodziu skaiciaus padidinima.
        inc ax
        mov [zodz], ax
        
        la9:            ; Ne tarpas, tad tarpo flaga nustatom 0. 
        and cl, 0FBh     ; 1111 1011
        
        la8:  ; tesiam cikla.
        inc si
        cmp si, bx              ; patikrinam ar dar ne buferio pabaiga
    jnz loopbuf1                ; sukam cikla kol cx'as ne 0.
    
    mov [statflags], cl         ; ikeliam cl'a i atminti ir pasiruosiam sekanciam blokui.
ret

startReadingFile: ; Failo skaitymo f-ja. 
;Failvardis (be nulio gale) - kintamajame 'filename'. Pask. simbolio pozicija - DI
	;cmp bx, [filestart] ; patikrinam ar naujas failvardis nera tuscias (sutampa pradzia su dab.pozicija)
    
    cmp byte ptr [filename], 0
	jz er1              
    cmp byte ptr [filename], 20h ; jei 0 arba tarpas, sokam i pabaiga
    jz er1
    
    ; Patikrinam ar pagalbos paramas (/?)
    cmp byte ptr [filename], '/'
    jnz startwork1
    cmp byte ptr [filename + 1], '?'
    jnz startwork1
        
    lea dx, help    ; atspausdinam pagalbos pran. ir baigiam.
    mov ah, 09h
    int 21h
    jmp programEnd
    
    ;jmp startwork1      ; jei viskas gerai, sokam i darbo pradzia.
    
    er1:
    jmp badFileNameErr  ; jei blogas failneimas.
    
    startwork1:    
    mov byte ptr [di + offset filename], '$' ; paruosiam failvardi spausdinimui - pabaigoje '$'
; === printing info stuff ===
    lea dx, endline    ; atspausdinam eil. pabaiga
    mov ah, 09h
    int 21h
    mov dl, 27h
    mov ah, 02h
	int 21h	
    
    lea dx, filename    ; atspausdinam failo varda is DS'o
    mov ah, 09h
    int 21h
    
    mov dl, 27h
    mov ah, 02h
	int 21h	
    mov dl, 20h
    mov ah, 02h
	int 21h	
  ; Filename print end.
	
	mov ax, di
	call printNumberDecimal     ; atspausdinam failo vardo ilgi
    
    ;lea dx, startopenfile ; print that we're starting to open a file.
    ;mov ah, 09h
    ;int 21h
; === end of cosmetic and info stuff ===
    
    ;mov byte ptr [di], 0 ; isnulinam vardo gala - pab.simboli tarpa pakeiciam 0 - buferio pabaiga.
    mov byte ptr [di + offset filename], 0 ; isnulinam vardo gala - pab.simboli tarpa pakeiciam 0 - buferio pabaiga.

    ;mov [filestart], bx       ; naujas failo startas - bx.
    
;======== Isvalom failo statistinius duomenis =========
    mov word ptr [simb], 0
    mov word ptr [zodz], 0
    mov word ptr [mazrai], 0
    mov word ptr [didrai], 0
    mov byte ptr [statflags], 04h ; default'e stovim ant whitespace'o.
;========  =========
    
    clc                     ; isvalom carry flag'a (Ten bus nustatoma klaida).
;======== Atidarom faila ========
    mov ah, 3dh             ; open file id
    lea dx, filename        ; put fname to dx
    mov al, 0               ; reading attrib
    int 21h                 ; call this function - handle on AX
    
    jc error1               ; jei klaida (carry flag=1), tai baigiam.
    mov [filehandle], ax    ; perkeliam handla i atminty.
;======== Atidarom faila pab. ======== 
    
;<DEBUG id="Atspausdinam Pradedam failo skaityma">
    ;lea dx, startread ; print that we're starting to read a file.
    ;mov ah, 09h
    ;int 21h
;</DEBUG>
    
;======== Failo skaitymo ciklas ==========
    readLoop1:
  ; Pradedam failo skaitynes!
    clc                     ; isvalom carry flag'a (Ten bus nustatoma klaida).
    ; perskaitom 64 baituku sektoriu.
    mov ah, 3Fh             ; skaitymo id
    mov bx, filehandle      ; bx'e - failo handlas
    mov cx, fileReadBuffLen ; kiek baitu skaitysim (64)
    lea dx, fileReadBuff    ; buferio adresas
    add dx, 2               ; padidinam 2, nes rasysim nuo 3 baito. (1,2-kiek perskaitem)
    int 21h                 ; kvieciam funkcija. AX'e - kiek baitu perskaitem.
    ; end of perskaitom 64 baituku sektoriu.
    ; tikrinam errorus ir tvarkom atminti. 
    jc error1               ; jei klaida (carry flag=1), tai baigiam.
    
    mov bx, [simb]          ; atnaujinam simboliu skaiciu.
    add bx, ax
    mov [simb], bx
    
    mov word ptr [fileReadBuff], ax  ; i 1 zodi bufe ikeliam kiek baitu perskaitem (is ax'o)
    cmp ax, 0               ; jei jau failo pabaiga (0 baitu perskaityta.)
    jz filend1
  ; Baigiam failo skaitynes.
    
    mov bx, ax
    mov byte ptr [bx + offset fileReadBuff + 2], '$' ; uzbaigiam failRydBufa su stringo terminatorium.
    
  ; Ivykdom egzaminacija (Updeitinam reiksmes statiniuse laukuose).
    call examineBuffer
    
    ;<DEBUG id="atspausdinam buferi">
        ;mov dx, offset fileReadBuff + 2
        ;mov ah, 09h
        ;int 21h
    ;</DEBUG>
    jmp readLoop1

;======== Failo skaitymo ciklo Pabaiga =========

;======== Failo uzdarymas ==========
    clc                     ; isvalom carry flag'a (Ten bus nustatoma klaida).
    mov bx, [filehandle]
    mov ah, 3Eh             ; uzdarymo id
    int 21h
    
    jc error1               ; jei klaida (carry flag=1), tai baigiam.  
    
    jmp endReadingFile      ; kai baigiam skaityt, persokam i f-jos pabaiga.
;======== Failo uzdarymas pab. ==========
    
    filend1:
    lea dx, endline    ; atspausdinam eil. pabaiga
    mov ah, 09h
    int 21h
    ;lea dx, filendstr
    ;mov ah, 09h
    ;int 21h
    
    mov ax, [filesread]
    inc ax
    mov [filesread], ax ; padidinam skaiciu perskaitytu failu.
    
    call printStats ; Atspausdinam failo statistinius duomenis
    jmp endReadingFile
    
    error1: ; klaidos informacijos atspausdinimas.
    lea dx, nofile
    mov ah, 09h
    int 21h
    call printNumberDecimal ; atspausdinam AX (klaidos koda) desimtainiu pavidalu
    lea dx, endline
    mov ah, 09h
    int 21h
    jmp endReadingFile
    
    badFileNameErr:
    mov ax, 0FFFFh  ; blogas failneimas
	
	endReadingFile:
	;mov ax, bx
	;inc ax
	;mov [filestart], di
    ;lea di, filename        ; DI vel ziures i failvardzio pradzia - rasysim is naujo.
    ;dec di                  ; atimam 1 nes paskui pridesim.
    mov di, -1      ; isnulinam di'ka    
ret

; Print file statistics:
printStats:
    ;lea dx, endline
    ;mov ah, 09h
    ;int 21h
    
    lea dx, rezultsimb
    mov ah, 09h
    int 21h
    mov ax, word ptr [simb]
    call printNumberDecimal
    
    lea dx, rezultzodz
    mov ah, 09h
    int 21h
    mov ax, word ptr [zodz]
    call printNumberDecimal
    
    lea dx, rezultmazrai
    mov ah, 09h
    int 21h
    mov ax, word ptr [mazrai]
    call printNumberDecimal
    
    lea dx, rezultdidrai
    mov ah, 09h
    int 21h
    mov ax, word ptr [didrai]
    call printNumberDecimal
    
    lea dx, endline
    mov ah, 09h
    int 21h
ret

; Functions end.

; ==============  void main() ===============
start:
	MOV ax, @data        ; perkelti data i registra ax
	MOV ds, ax           ; perkelti ax (data) i data segmenta
    
    mov bx, 81h          ; bx'as - paramo pradzia.
    xor di, di
        
	
	tonext:
        mov dl, es:[bx]      ; i dl'a ikeliam simboli is extended sego (parametro simboli)
        cmp dl, 0dh
        jz  loopend			 ; patikrinam ar dar nesibaige, jei taip, baigiam cikliuka

        ;mov ah, 02h
        ;int 21h				 ; atspausdinam simboli
    
        mov byte ptr [di + offset filename], dl ; i failo vardo pozicija di ikeliam dab. simboli.
	
        cmp dl, 20h			 ; patikrinam ar ne tarpas, jei taip, pradedam failo skaitynes.
        jnz after1
        
        push bx              ; idedam bx'a i steka, nes funkcijoje bx'as bus naud0jamas.
        call startReadingFile 
        pop bx
    
        after1:
        inc bx
        inc di
	jmp tonext			 ; tesiam cikla.
    
	loopend:    
	call startReadingFile ; automatiskai pradedam skaityt, nes jei baigem tai pask.paramo failas dar neperskaitytas
    
    ; Patikrinam ar persk. failu skaicius nera 0
    cmp word ptr [filesread], 0
    jnz programEnd    ; jei perskaitem bent 1 faila, sokam i programos pabaiga.
    
    ; jei ne,
  ;========== Skaitom is konsoles ==========
    lea dx, badparams_std   ; atspausdinam: rasyk i konsole.
    mov ah, 09h
    int 21h
    
    ; Paruosiam buferi vartojimui.
    mov byte ptr [fileReadBuff], 255    ; 1 pozicijoje kiek max. charu buferis gali laikyt.
                                        ; 2 pozicijoje kiek perskaitem is tiesu.
                                        ; Siuo atveju bufas gali laikyt 255 charus.
    ; n.
    mov ah, 0Ah
    lea dx, fileReadBuff
    int 21h                             ; kvieciam funkcija skaitymui is buferio.
    
    xor bx, bx
    mov bl, [fileReadBuff + 1]
    mov byte ptr [fileReadBuff], bl
    mov byte ptr [fileReadBuff + 1], 0      ; isnulinam antra baita, nes musu egzaminatorius ims baitu kieki is 1 zodzio,
                                            ; o DOSas persk.baitu kieki idejo i 2 baita.
                                            
    mov [simb], bx
    
    ;<debug id="atspausdinam buf.">
    ;mov byte ptr [bx + offset fileReadBuff + 2], '$'
    
    ;mov dx, offset fileReadBuff + 2
    ;mov ah, 09h
    ;int 21h
    
    lea dx, endline
    mov ah, 09h
    int 21h
    ;</debug>
    
    ; Ivykdom egzaminacija (Updeitinam reiksmes statiniuse laukuose). Automatiskai skaitysim is 'fileReadBuff'
    call examineBuffer
    ; Atspausdinam gauta statistika.
    call printStats
    
  ;==========  ==========
    
    ;mov dl, '$'
	;mov ah, 02h
	;int 21h	
    
    programEnd:
	mov 	ah, 4ch 	; sustabdyti programa - http://www.computing.dcu.ie/~ray/teaching/CA296/notes/8086_bios_and_dos_interrupts.html#int21h_4Ch
	mov 	al, 0 		; be klaidu = 0
	int 	21h         ; 21h -  dos pertraukimmas - http://www.computing.dcu.ie/~ray/teaching/CA296/notes/8086_bios_and_dos_interrupts.html#int21h
end start

