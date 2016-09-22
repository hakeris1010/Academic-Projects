; Programa: Nr. 1
; Uzduoties salyga: Programa iveda eilute ir isveda pozicijas simboliu sutampanciu su pirmuoju.
; Atliko: 

.model small
.stack 100h
	 
.data
	msg1		db 'Programa iveda eilute ir isveda pozicijas simboliu sutampanciu su pirmuoju.', 0dh, 0ah, 'Iveskite eilute:', 0dh, 0ah, '$'
	result    	db 0Dh, 0Ah, 'Rezultatas:', 0Dh, 0Ah, '$'
	buffer		db 20, ?, 20 dup (0)
	output		db 6 dup (32), '$'

.code
 
start:
	MOV ax, @data                   ; perkelti data i registra ax
	MOV ds, ax                      ; perkelti ax (data) i data segmenta
	 
	; Isvesti uzklausa
	MOV ah, 09h
	MOV dx, offset msg1
	int 21h

	; skaityti eilute
	MOV ah, 0Ah                     ; eilutes skaitymo subprograma
	MOV dx, offset buffer           ; skaityti i buffer offseta 
	INT 21h                         ; dos'o INTeruptas

	; nustatom duomenis
	MOV si, offset buffer           ; priskirti source index'ui bufferio koordinates
	INC si                          ; pridedam 1 prie si , nes pirmas kiek simboliu ish viso
	MOV bh, [si]                    ; idedam i bh kiek simboliu nuskaitem
	INC si                          ; pereiname prie pacio pirmo simbolio
	MOV bl, [si]					; i bl idedam pirma simboli 
	
									; steko metodas blogai veikia, tad naudosim output buffer'io metoda.
	;push byte '$' 					; [bad] i steka idedam eilutes pabaigos simboli - reikes rezultu spauzdinimui.
	 
	; isvesti: rezultatas
	MOV ah, 09h
	MOV dx, offset result
	int 21h

	; patikrinam ar nuskaitem kiek reikia
	cmp bh, 0
	jz ending
	cmp bl, 0dh
	jz ending
	
charloop:							;sukam cikla
	cmp bl, byte ptr[si]
	jz get_pos
advance:
	inc si
	dec bh
	jz ending						; jei bh'as jau 0 (eil.pab), baigiam.
	jmp charloop					; jei ne, sokam i kita simboli.
	
; funkcija getpos, gauna pozicija ir konvertuoja i desimtaini pavidala ir atspauzdina.
get_pos:
	mov ax, si						; i ax'a idedam pozicija eiluteje
	sub ax, offset buffer + 2 		
	mov di, offset output + 5		; di - pozas buferyje kur desim vienetus.
	mov cx, 10						; dalinsim is 10.
	
loopit:								; konvertuojam i 10-aini.
	xor dx, dx						; isvalom dx'a (ten bus liekana)
	div cx							; padalinam pos'a (ax) is 10 (cx). dalinys lieka ax'e, o remainderis i dx'a.
	add dl, '0'						; paverciam i char'a
	
	;push byte dl					; [bad] idedam skaiciuka i steka.
	mov [di], dl					; buf.metodas - idedam skaitmeni i buferio pozicija [di]
	dec di
	
	cmp ax, 0						; paziurim ar dar nesibaige skaicius.
	jnz loopit						; jei ne, tai einam i sekanti skaiciukas.
	
	;push byte ' '					; [bad] kai baigesi, idedam tarpa.
	xor ax, ax						; atspausdinam output'a kai baigiam konvertacija
	mov ah, 09h
	mov dx, di											
	int 21h
	
	jmp advance						; griztam i pagr.tikrintuva.
	 
ending: ; pabaigoje atspausdinam eilute is steko, ir uzbaigiam programa.
	;xor ax, ax
	;mov ah, 09h
	;mov dx, sp						; steko pabaiga					
	;int 21h

	MOV ax, 4c00h 		       		; griztame i dos'a
	INT 21h                        	; dos'o INTeruptas
	 
end start