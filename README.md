# Dining-Philosophers
使用Pthreads mutex locks 和 condition variables解决哲学家就餐问题。

避免死锁策略为：
哲学家 i 可以进行就餐，当且仅当她的两个邻座不处于就餐状态，即(state[(i + 4) % 5] != EATING)&&(state[(i+1) % 5] != EATING)

