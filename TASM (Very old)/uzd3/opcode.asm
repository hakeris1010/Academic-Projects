.data

;------- OpCodes & Mnemonics -------;
; Entry structure (Lenght-10):
; - Byte 1: OpCode.
; - Byte 2: Flags:
;   + Bit 0: OpCode+R type (Reg field in OpCode Low-Order Nibble).
;   + Bit 1: ModR/M byte present
;   + Bit 2: D bit replaced by S
;   + Bit 3: Extended Opcode (RM/Immediate instructions. REG field acts as an extension.)
;   + Bit 4: Special OpCode (Needs further inspection) (W and/or D bits are part of OpCode)
;   + Bit 5: D bit is not used
;   + Bit 6: W bit is not used
;   + Bit 7: Prefix.
;       If Prefix, then:
;       + Bit 0: Segment Override
; - Byte 3-10: OpCode Mnemonic String (8 bytes).
;
t_OPCODE_TABLE  dw 9, 10   ; Entries, Entry Lenght
i_op_MOV_rm_r           db 88h , 02h, 'MOV $   '

i_op_MOV_r8_imm8a_R     db 0B0h, 21h, 'MOV $   '
i_op_MOV_r8_imm8b_R     db 0B4h, 21h, 'MOV $   '
i_op_MOV_r16_imm16a_R   db 0B8h, 21h, 'MOV $   '
i_op_MOV_r16_imm16b_R   db 0BCh, 21h, 'MOV $   '

i_op_MOV_rm_imm         db 0C6h, 2Eh, 'MOV $   '

; Special Opcodes. Should be placed in special table with special flags, along with Prefixes. 
; E.g. Bit 7 : Special Opcode. Then, bit 1 : Prefix, bit 2 : No W flag, etc.
; t_SPECIAL_OPC_TABLE dw 2, 10
i_op_MOV_rm_Sreg        db 8Ch , 52h, 'MOV $   '
i_op_MOV_Sreg_rm        db 8Eh , 52h, 'MOV $   '
i_op_MOV_rALX_moff      db 0A0h, 14h, 'MOV $   '

; Prefixes
; Flag Byte is different.
i_op_PREF_TABLE  dw 7, 10
i_pr_REP                db 0F3h, 80h, 'REP $   '
i_pr_REPNE              db 0F2h, 80h, 'REPNE $ '
i_pr_CS                 db 2Eh,  81h, 'CS:$    '
i_pr_DS                 db 3Eh,  81h, 'DS:$    '
i_pr_ES                 db 26h,  81h, 'ES:$    '
i_pr_SS                 db 36h,  81h, 'SS:$    '
i_pr_LOCK               db 0F0h, 80h, 'LOCK $  '


;------- Adressing Byte Modes -------;
; ModRM Tables:
; + Word 1: Entries in a table
; + Word 2: Entry lenght.
;
; REG Table : Use in R/M if MOD = 11. W bit is USED!
; Used as REG always, and as R/M if MOD=11.
; - Entry lenght: 7 bytes.
; - Byte 1: W+REG field value in a byte
; - Bytes 2-7: String value of operand 
;              W+REG  Mnemonic
t_REG dw 16, 7
t_REG_START db  00h, 'AL$   ',\ ; 8 bit ones (W=0)
                01h, 'CL$   ',\
                02h, 'DL$   ',\
                03h, 'BL$   ',\
                04h, 'AH$   ',\
                05h, 'CH$   ',\
                06h, 'DH$   ',\
                07h, 'BH$   ',\
\
                08h, 'AX$   ',\ ; 16 bit ones (W=1)
                09h, 'CX$   ',\
                0Ah, 'DX$   ',\
                0Bh, 'BX$   ',\
                0Ch, 'SP$   ',\
                0Dh, 'BP$   ',\
                0Eh, 'SI$   ',\
                0Fh, 'DI$   '
                

; Other tables. W bit is not used.
; - Entry lenght: 7 bytes.
; - Byte 1: REG/RM field value in a byte
; - Bytes 2-7: String value of operand
;
; SREG Table : Only in REG field.
;               REG   Value
t_SREG dw 4, 7
t_SREG_START db 00h, 'ES$   ',\
                01h, 'CS$   ',\
                02h, 'SS$   ',\
                06h, 'DS$   '

; R/M Tables 1-2 : MOD=00,01,10
; Cases with MOD:
; - MOD=00 : (Table 1) No displacement, 06H=Drct.Ad.
; - MOD=01 : (Table 2) 8-bit displacement,  06H='BP'
; - MOD=10 : (Table 2) 16-bit displacement, 06H='BP'.
t_RM12 dw 8, 7
t_RM12_START db 00h, 'BX+SI$',\
                01h, 'BX+DI$',\
                02h, 'BP+SI$',\
                03H, 'BP+DI$',\
                04H, 'SI$   ',\
                05H, 'DI$   ',\
                06H, 'BP$   ',\ ; OR Direct Addressing if Table 1
                07H, 'BX$   '
          
; MOD Field table.
; + Entry lenght: 2 bytes.
; - MOD: mod field value.
; - Table no.: 0 - R/M table 1 (No disp.)
;              1 - R/M table 2, 8-bit disp.
;              2 - R/M table 2, 16-bit disp.
;              3 - REG or SREG table
; 
; MOD | Table no.
t_MOD db  4, 2,\
          00h, 0,\
          01h, 1,\
          02h, 2,\
          03h, 3

