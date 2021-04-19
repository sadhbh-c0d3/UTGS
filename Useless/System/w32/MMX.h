#ifndef __INLUCDED__USELESS_MMX_H__
#define __INLUCDED__USELESS_MMX_H__

namespace Useless {

inline void MemCopy( void *dest, void *src, const int width )
{
    __asm{
            emms
            mov esi, src
            mov edi, dest
            mov ecx, width
            mov eax, ecx
            shr eax, 3
            cmp eax, 0
            jz  done_64
        do_64:

            movq mm0, [esi]
            movq [edi], mm0
            add esi, 8
            add edi, 8
            sub ecx, 8
            dec eax
            jnz do_64

        done_64:
            mov eax, ecx
            shr eax, 2
            cmp eax, 0
            jz  done_32
            
            mov ebx, [esi]
            mov [edi], ebx
            add esi, 4
            add edi, 4
            sub ecx, 4

        done_32:
            mov eax, ecx
            shr eax, 1
            cmp eax, 0
            jz  done_16
            
            mov  bx, [esi]
            mov [edi], bx
            add esi, 2
            add edi, 2
            sub ecx, 2

        done_16:
            cmp ecx, 0
            jz  done

            mov bl, [esi]
            mov [edi], bl
    
        done:
            emms
    }
}

inline void MemFill( void *dest, const int width )
{
    __asm{
            emms

			xor  ebx, ebx
			push ebx
			push ebx
            movq mm0, [esp]
			add esp, 8

            mov edi, dest
            mov ecx, width
            mov eax, ecx
            shr eax, 3
            cmp eax, 0
            jz  done_64
        do_64:

            movq [edi], mm0
            add esi, 8
            add edi, 8
            sub ecx, 8
            dec eax
            jnz do_64

        done_64:
            mov eax, ecx
            shr eax, 2
            cmp eax, 0
            jz  done_32
            
            mov [edi], ebx
            add esi, 4
            add edi, 4
            sub ecx, 4

        done_32:
            mov eax, ecx
            shr eax, 1
            cmp eax, 0
            jz  done_16
            
            mov [edi], bx
            add esi, 2
            add edi, 2
            sub ecx, 2

        done_16:
            cmp ecx, 0
            jz  done

            mov [edi], bl
    
        done:
            emms
    }
}


};//namespace Useless
#endif//__INLUCDED__USELESS_MMX_H__
