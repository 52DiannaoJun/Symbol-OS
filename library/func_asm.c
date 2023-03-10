static u32 sys_memcheck_sub(u32 start, u32 end) {
  u32 i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;

  for (i = start; i <= end; i += 0x1000) {
    p = (u32 *)(i + 0xffc);

    old = *p;         // 先记住修改前的值
    *p = pat0;        // 试写
    *p ^= 0xffffffff; // 反转

    if (*p != pat1) {
      // 检查反转结果
    not_memory:
      *p = old;
      break;
    }

    *p ^= 0xffffffff; // 再次反转
    if (*p != pat0) {
      // 检查值是否恢复
      goto not_memory;
    }
    *p = old;
  }

  return i;
}
static u32 sys_memcheck(u32 start, u32 end) {
  char flg486 = 0;
  u32 eflg, cr0;

  eflg = load_eflags();
  eflg |= EFLAGS_AC_BIT;

  store_eflags(eflg);
  eflg = load_eflags();
  if ((eflg & EFLAGS_AC_BIT)) {
    // 如果是i386，即使设定AC=1，也会自动变回到0
    flg486 = 1;
  }

  eflg &= ~EFLAGS_AC_BIT;
  store_eflags(eflg);

  if (flg486) {
    cr0 = load_cr0();
    cr0 |= CR0_CACHE_DISABLE; // 禁止缓存
    store_cr0(cr0);
  }

  eflg = sys_memcheck_sub(start, end);

  if (flg486) {
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE; // 允许缓存
    store_cr0(cr0);
  }

  return eflg;
}
static void delay (u32 times){
  for(u32 i=1;i<times;i++)
    asm_nop();
  return;
}