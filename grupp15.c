#define MOVE_TIME 2000
#define TURN_TIME 1000
task main()
{
repeat(4)
{
OnFwd(OUT_AC, 75);
Wait(MOVE_TIME);
OnRev(OUT_C, 75);
Wait(TURN_TIME);
}
Off(OUT_AC);
}
