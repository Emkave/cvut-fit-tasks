.data		# determine and set prime numbers
a00000:		.word 1, 2, 3, 4, 5
.text
af0000:     addi t0, x0, 0
af0004:     addi t1, x0, 5
af0008:     la s0, a00000
af000c:     lw a0, 0(s0)     
af0010:     addi t2, x0, 2      
af0014:     blt a0, t2, bf0038   
af0018:     addi t2, x0, 3
af001c:     beq a0, t2, bf0010
af0020:     addi t2, x0, 2       
af0023:     addi t6, x0, 2       
af0024:     div t3, a0, t6       
af0028:     slt t4, t2, t3       
af002c:     addi t5, x0, 1
af0030:     beq t4, t5, af0104   
af0034:     beq t2, t3, af0104   
af0038:     j bf0010             
af0104:     rem t6, a0, t2       
af0108:     beq t6, x0, bf0038   
af010c:     addi t2, t2, 1      
af0110:     j af0028 
aff000:     sw a0, 0(s0)         
aff004:     addi t0, t0, 1       
aff008:     addi s0, s0, 4       
aff00c:     blt t0, t1, af000c   
aff010:     j ffffff             
bf0010:     addi a0, x0, 1       
bf0014:     j aff000       
bf0038:     addi a0, x0, 0       
bf003c:     j aff000
ffffff:     
