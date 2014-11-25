//extern "C"
//{

float __declspec(naked) _CIcos()
{
   _asm {
      fcos
      ret
   };
};



//};

