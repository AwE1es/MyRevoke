using System;
using System.Runtime.CompilerServices;


namespace Revoke
{

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void CppFunction();
    }

    public  class Entity
    {
        public float FloatVar {  get; set; }
        public Entity()
        {
            Console.WriteLine("Maim constructor");
            InternalCalls.CppFunction();
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