global io_wait
global outb
global outw
global inb
global inw

section .text

; Создаёт задержку в 1 микросекунду
io_wait:
        mov dx, 0x80
        mov al, 0
        out dx, al
        ret

outb:
        push ebp
        mov ebp, esp

        mov dx, [ebp + 8]
        mov al, [ebp + 12]

        out dx, al

        pop ebp
        ret

outw:
        push ebp
        mov ebp, esp

        mov dx, [ebp + 8]
        mov ax,  [ebp + 12]

        out dx, ax

        pop ebp
        ret

inb:
        push ebp
        mov ebp, esp

        mov dx, [ebp + 8]

        in al, dx

        pop ebp
        ret

inw:
        push ebp
        mov ebp, esp

        mov dx, [ebp + 8]

        in ax, dx

        pop ebp
        ret
