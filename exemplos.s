
.text

# EXEMPLO 1: retorna uma constante
.globl exemplo1
exemplo1:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl $100, %eax

    leave
    ret


# EXEMPLO 2: f(x) = x + 1
.globl exemplo2
exemplo2:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl %edi, -4(%rbp)       # guarda p0
    movl -4(%rbp), %eax
    addl $1, %eax
    movl %eax, -8(%rbp)       # guarda v0

    movl -8(%rbp), %eax

    leave
    ret


# EXEMPLO 3: fatorial
.globl fatorial
fatorial:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl %edi, -4(%rbp)       # p0

    # zret p0 $1
    cmpl $0, -4(%rbp)
    jne .Lfact_rec
    movl $1, %eax
    leave
    ret

.Lfact_rec:
    # v0 = p0 + $0
    movl -4(%rbp), %eax
    addl $0, %eax
    movl %eax, -8(%rbp)

    # v1 = v0 - $1
    movl -8(%rbp), %eax
    subl $1, %eax
    movl %eax, -12(%rbp)

    # v1 = call 0 v1
    movl -12(%rbp), %edi
    call fatorial
    movl %eax, -12(%rbp)

    # v0 = v0 * v1
    movl -8(%rbp), %eax
    imull -12(%rbp), %eax
    movl %eax, -8(%rbp)

    # ret v0
    movl -8(%rbp), %eax

    leave
    ret


# EXEMPLO 4: soma dos quadrados

# Primeira função: calcula o quadrado
.globl quadrado
quadrado:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl %edi, -4(%rbp)

    # v0 = p0 * p0
    movl -4(%rbp), %eax
    imull -4(%rbp), %eax
    movl %eax, -8(%rbp)

    # ret v0
    movl -8(%rbp), %eax

    leave
    ret


# Segunda função: soma dos quadrados de 1 até p0
.globl soma_quadrados
soma_quadrados:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl %edi, -4(%rbp)

    # zret p0 $0
    cmpl $0, -4(%rbp)
    jne .Lsoma_rec
    movl $0, %eax
    leave
    ret

.Lsoma_rec:
    # v0 = p0 - $1
    movl -4(%rbp), %eax
    subl $1, %eax
    movl %eax, -8(%rbp)

    # v1 = call 0 p0
    movl -4(%rbp), %edi
    call quadrado
    movl %eax, -12(%rbp)

    # v0 = call 1 v0
    movl -8(%rbp), %edi
    call soma_quadrados
    movl %eax, -8(%rbp)

    # v0 = v0 + v1
    movl -8(%rbp), %eax
    addl -12(%rbp), %eax
    movl %eax, -8(%rbp)

    # ret v0
    movl -8(%rbp), %eax

    leave
    ret
