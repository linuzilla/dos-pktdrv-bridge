; Packet Driver
;
                .286
                .MODEL  SMALL, C
                .CODE
                PUBLIC  MacAddrCMP
MacAddrCMP      PROC    NEAR USES ds es cx si di, mac1: DWORD, mac2: DWORD
                les     di, [mac2]
                lds     si, [mac1]
                mov     cx, 3
        rep     cmpsw
                mov     ax, 0
                jnz     exit_MacAddrCMP
                inc     ax
exit_MacAddrCMP:
                ret
MacAddrCMP      ENDP

                END
