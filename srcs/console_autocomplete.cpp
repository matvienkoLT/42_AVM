#include "console_autocomplete.hpp"
#include <vector>

const char * CommandHelper::kPushKey = "push";
const std::vector<const char*> PUSH_VARIANT = {"push", "push int8(", "push int16(",
                              "push int32(", "push float(",
                              "push double("};

const char * CommandHelper::kDumpKey = "dump";
const char * CommandHelper::kPOPKey = "pop";

static size_t i = 0;

struct termios CommandHelper::old, CommandHelper::current;

CommandHelper::CommandHelper(void) : _ptr_buffer(nullptr)
{
}

CommandHelper::CommandHelper(std::string &buffer) : _ptr_buffer(&buffer)
{
}

/* Initialize new terminal i/o settings */
void CommandHelper::termiOSInit(int echo) 
{
    /* grab old terminal i/o settings */
    tcgetattr(0, &old);
  
    /* make new settings same as old settings */
  current = old;

    /* disable buffered i/o */
  current.c_lflag &= ~ICANON;

  if (echo)
  {
        current.c_lflag |= ECHO; /* set echo mode */
  }
  else
  {
        current.c_lflag &= ~ECHO; /* set no echo mode */
  }
    tcsetattr(0, TCSANOW, &current);/* use these new terminal i/o settings now */
}

void    CommandHelper::resetTermiOS(void)
{
  tcsetattr(0, TCSANOW, &old);
}

char    CommandHelper::getch_(int echo) 
{
  char ch;
  termiOSInit(echo);
  ch = getchar();
  resetTermiOS();
  return ch;
}

/* Read 1 character without echo */
char    CommandHelper::getch(void) 
{
  return getch_(0);
}

char    CommandHelper::getche(void) 
{
  return getch_(1);
}

static int take_another_variant(std::string &ss)
{
  if (i >= PUSH_VARIANT.size())
    i = 0;
  
  for(; i < PUSH_VARIANT.size(); ++i)
  {
      if (ss == (std::string)PUSH_VARIANT[i] || ((std::string)PUSH_VARIANT[i]).find(ss))
      {
        i = i + 1;
        return i >= PUSH_VARIANT.size() ? 0 : i;
      }
  }
  return -1;
}

int CommandHelper::loop_listener(void)
{
    char alpha;


    if (_ptr_buffer)
    {
        alpha = getch();

      //std::cout<<(int)alpha<<std::endl;
        if (alpha == 0x7f)
      {
        std::cout << (char)0x08 << ' ' << (char)0x08;

        if (!_ptr_buffer->empty())
          _ptr_buffer->pop_back();
        return 0;
      }
      else if (alpha == 9)
      {
        size_t position;

        int curr_index = take_another_variant(*_ptr_buffer);
        if(curr_index != -1)
        {
          for(size_t a(0); a < _ptr_buffer->size(); a++)
          {
             std::cout << (char)0x08 << ' ' << (char)0x08;
          }
            std::cout << ((std::string)PUSH_VARIANT[curr_index]);
          *_ptr_buffer = ((std::string)PUSH_VARIANT[curr_index]);  
        }
        else if ((position =((std::string)kPushKey).find(*_ptr_buffer)) != std::string::npos)
        {
          std::cout << ((std::string)kPushKey).substr(_ptr_buffer->length());
          *_ptr_buffer = ((std::string)kPushKey);
        }
        else if ((position =((std::string)kDumpKey).find(*_ptr_buffer)) != std::string::npos)
        {
          std::cout << ((std::string)kDumpKey).substr(_ptr_buffer->length());
          *_ptr_buffer = ((std::string)kDumpKey);
        }
        else if ((position =((std::string)kPOPKey).find(*_ptr_buffer)) != std::string::npos)
        {
          std::cout << ((std::string)kPOPKey).substr(_ptr_buffer->length());
          *_ptr_buffer = ((std::string)kPOPKey);
        }
        return 0;
      }
      else if (alpha != 0xa)
      {
        if (alpha == '\033')
        {
          char kk = getch();
          getch();
          switch(kk) { // the real value
          case 'A':
             return 0;
          case 'B':
              return 0;
          case 'C':
              return 0;
          case 'D':
              return 0;
          default:
              return 0;
          }
        }
        std::cout<<alpha<<std::flush;
        _ptr_buffer->push_back(alpha);
        return 0;
      }
      else
        std::cout<<std::endl;
      return 1;
    }
    return (-1);
}