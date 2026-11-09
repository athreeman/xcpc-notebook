
//1、
//正整数a,b
//若存在a%c=b
//则一定满足a=b,或a>2*b
//正整数a可以得到的所有余数[0...(a-1)/2]&&[a]
//向下取整

//2、
//最大公约数相关
//任意正整数a<b
//都有gcd(a,b)=gcd(a,b-a)

//3、
//对于任意正整数a<b
//若a%m=b%m,则(b-a)%m=0


//4、
// 若(a+b)%mod=0
// 给定a,则b%mod=(mod-a%mod)%mod

//5、
// (a+b)%mod==j
// 给定a,b%mod=((j-a)%mod+mod)%mod

//6、
// (a%m*b%m)%m=(a*b)%mod
// -a%mod=(-a%mod+mod)%mod=(mod-a)%mod