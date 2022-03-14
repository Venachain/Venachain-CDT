; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -fast-isel -mtriple=i686-unknown-unknown -mattr=+avx512cd,+avx512vl | FileCheck %s --check-prefixes=CHECK,X86
; RUN: llc < %s -fast-isel -mtriple=x86_64-unknown-unknown -mattr=+avx512cd,+avx512vl | FileCheck %s --check-prefixes=CHECK,X64

define <2 x i64> @test_mm_broadcastmb_epi64(<2 x i64> %a, <2 x i64> %b) {
; X86-LABEL: test_mm_broadcastmb_epi64:
; X86:       # %bb.0: # %entry
; X86-NEXT:    vpcmpeqd %xmm1, %xmm0, %k0
; X86-NEXT:    kmovw %k0, %eax
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    vmovd %eax, %xmm0
; X86-NEXT:    vpbroadcastq %xmm0, %xmm0
; X86-NEXT:    retl
;
; X64-LABEL: test_mm_broadcastmb_epi64:
; X64:       # %bb.0: # %entry
; X64-NEXT:    vpcmpeqd %xmm1, %xmm0, %k0
; X64-NEXT:    vpbroadcastmb2q %k0, %xmm0
; X64-NEXT:    retq
entry:
  %0 = bitcast <2 x i64> %a to <4 x i32>
  %1 = bitcast <2 x i64> %b to <4 x i32>
  %2 = icmp eq <4 x i32> %0, %1
  %3 = shufflevector <4 x i1> %2, <4 x i1> zeroinitializer, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  %4 = bitcast <8 x i1> %3 to i8
  %conv.i = zext i8 %4 to i64
  %vecinit.i.i = insertelement <2 x i64> undef, i64 %conv.i, i32 0
  %vecinit1.i.i = shufflevector <2 x i64> %vecinit.i.i, <2 x i64> undef, <2 x i32> zeroinitializer
  ret <2 x i64> %vecinit1.i.i
}

define <4 x i64> @test_mm256_broadcastmb_epi64(<4 x i64> %a, <4 x i64> %b) {
; X86-LABEL: test_mm256_broadcastmb_epi64:
; X86:       # %bb.0: # %entry
; X86-NEXT:    vpcmpeqq %ymm1, %ymm0, %k0
; X86-NEXT:    kmovw %k0, %eax
; X86-NEXT:    movzbl %al, %eax
; X86-NEXT:    vmovd %eax, %xmm0
; X86-NEXT:    vpbroadcastq %xmm0, %ymm0
; X86-NEXT:    retl
;
; X64-LABEL: test_mm256_broadcastmb_epi64:
; X64:       # %bb.0: # %entry
; X64-NEXT:    vpcmpeqq %ymm1, %ymm0, %k0
; X64-NEXT:    vpbroadcastmb2q %k0, %ymm0
; X64-NEXT:    retq
entry:
  %0 = icmp eq <4 x i64> %a, %b
  %1 = shufflevector <4 x i1> %0, <4 x i1> zeroinitializer, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  %2 = bitcast <8 x i1> %1 to i8
  %conv.i = zext i8 %2 to i64
  %vecinit.i.i = insertelement <4 x i64> undef, i64 %conv.i, i32 0
  %vecinit3.i.i = shufflevector <4 x i64> %vecinit.i.i, <4 x i64> undef, <4 x i32> zeroinitializer
  ret <4 x i64> %vecinit3.i.i
}

define <2 x i64> @test_mm_broadcastmw_epi32(<8 x i64> %a, <8 x i64> %b) {
; CHECK-LABEL: test_mm_broadcastmw_epi32:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpcmpeqd %zmm1, %zmm0, %k0
; CHECK-NEXT:    vpbroadcastmw2d %k0, %xmm0
; CHECK-NEXT:    vzeroupper
; CHECK-NEXT:    ret{{[l|q]}}
entry:
  %0 = bitcast <8 x i64> %a to <16 x i32>
  %1 = bitcast <8 x i64> %b to <16 x i32>
  %2 = icmp eq <16 x i32> %0, %1
  %3 = bitcast <16 x i1> %2 to i16
  %conv.i = zext i16 %3 to i32
  %vecinit.i.i = insertelement <4 x i32> undef, i32 %conv.i, i32 0
  %vecinit3.i.i = shufflevector <4 x i32> %vecinit.i.i, <4 x i32> undef, <4 x i32> zeroinitializer
  %4 = bitcast <4 x i32> %vecinit3.i.i to <2 x i64>
  ret <2 x i64> %4
}

define <4 x i64> @test_mm256_broadcastmw_epi32(<8 x i64> %a, <8 x i64> %b) {
; CHECK-LABEL: test_mm256_broadcastmw_epi32:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpcmpeqd %zmm1, %zmm0, %k0
; CHECK-NEXT:    vpbroadcastmw2d %k0, %ymm0
; CHECK-NEXT:    ret{{[l|q]}}
entry:
  %0 = bitcast <8 x i64> %a to <16 x i32>
  %1 = bitcast <8 x i64> %b to <16 x i32>
  %2 = icmp eq <16 x i32> %0, %1
  %3 = bitcast <16 x i1> %2 to i16
  %conv.i = zext i16 %3 to i32
  %vecinit.i.i = insertelement <8 x i32> undef, i32 %conv.i, i32 0
  %vecinit7.i.i = shufflevector <8 x i32> %vecinit.i.i, <8 x i32> undef, <8 x i32> zeroinitializer
  %4 = bitcast <8 x i32> %vecinit7.i.i to <4 x i64>
  ret <4 x i64> %4
}


