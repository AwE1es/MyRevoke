using System;


namespace Revoke
{

    public class Main
    {
        public float FloatVar {  get; set; }
        public Main()
        {
            Console.WriteLine("Maim constructor");
        }
    
        public void PrintMessege()
        {
            Console.WriteLine("Hellloooooooooo");
        }
    
        public void PrintCustomMessege(int message)
        {
            Console.WriteLine($"Hellloooooooooo : {message}");
        }
    }
}