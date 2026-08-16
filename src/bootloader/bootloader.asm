org 0x7C00
bits 16

; Константы
; -----------------------------------------------------------------------
CODE_OFFSET equ 0x8          ; Смещение сегмента кода в GDT (8 байт)
DATA_OFFSET equ 0x10         ; Смещение сегмента данных в GDT (16 байт)

KERNEL_START_ADDR equ 0x1000 ; Адрес где начинаается ядро (1КБ)
; -----------------------------------------------------------------------

start:
        cli

        mov sp, 0x7C00          ; Указатель на начало стека (растёт в низ до 0x0000)

        mov ax, 0x00
        mov es, ax
        mov ds, ax
        mov es, ax
        mov ss, ax

        sti

        ; Загрузка ядра
        mov bx, KERNEL_START_ADDR ; Где начинается ядро
        mov dl, 0x80              ; Читаем с первого жесткого диска
        mov dh, 0x00              ; Читаем с нулевой головки
        mov ch, 0x00              ; Читаем с нулевого цилиндра
        mov cl, 0x02              ; Читаем с второго cектора
        mov al, 8                 ; Читаем 8 секторов
        mov ah, 0x02              ; Функция чтения BIOS
        int 0x13

        jc load_kernel_error
        jmp load_pm

load_kernel_error:
        mov si, load_kernel_err  ; Загружаем адрес ошибки в si
.loop:
        lodsb                    ; Записывает байт из si в al и увеличивает адрес на 1 байт
        or al, al                ; Проверака на нуль-терминатор
        jz .done                  ; Если al == 0, выход из цикла
        
        mov ah, 0x0E             ; Номер функции вывода
        mov bh, 0x00             ; Номер видеостранницы
        mov bl, 0x07             ; Цвет
        int 0x10
        jmp .loop
.done:
        cli
        hlt

load_pm:
        cli

        ; Включение линии A20 (позволяет использовать более 1МБ ОЗУ)
        in al, 0x92
        or al, 2
        out 0x92, al

        ; Загрузка GDT
        lgdt [gdt_descriptor]

        ; Включение Protected Mode
        mov eax, cr0
        or al, 1
        mov cr0, eax
        jmp CODE_OFFSET:pm_main ; В cs записывается селектор CODE_OFFSET, в ip/eip адрес pm_main
                                 ; Процессор начинает выполнение 32-битного кода по адресу pm_main
        
align 4
gdt_start:
        ; Null дескриптор
        dq 0x0000000000000000

        ; Code segment дескриптор
        dw 0xFFFF      ; Limit
        dw 0x0000      ; Base
        db 0x00        ; Base
        db 10011010b   ; Access byte
        db 11001111b   ; Flags
        db 0x00        ; Base
        
        ; Data segment дескриптор
        dw 0xFFFF      ; Limit
        dw 0x0000      ; Base
        db 0x00        ; Base
        db 10010010b   ; Access byte
        db 11001111b   ; Flags
        db 0x00        ; Base
gdt_end:

gdt_descriptor:
        dw gdt_end - gdt_start - 1
        dd gdt_start

load_kernel_err db "Load kernel error...", 0

bits 32
pm_main:
        mov ax, DATA_OFFSET
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov ss, ax
        mov gs, ax

        ; Инициализация стека
        mov ebp, 0x9C00
        mov esp, ebp

        ; Загрузчик С-ядра
        mov eax, KERNEL_START_ADDR
        jmp eax

times 510 - ($ - $$) db 0
dw 0xAA55
