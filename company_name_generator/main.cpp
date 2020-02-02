#include <iostream>
#include <vector>

int main()
{

    std::vector<std::string> list_of_names = {"viking",
                                              "nordic", "nord", "wave", "plugin", "penguin", "frost", "odin", "forge", "byte", "synth", "VST", "audio", "sound", "bit", "sonic", "algebra", "admin", "numerical", "science", "architect", "astronaut", "clerk", "engineer", "Sculptor", "surgeon", "silicon", "processor", "signal"};

    for (int x = 0; x < list_of_names.size(); ++x)
    {
        for (int y = 0; y < list_of_names.size(); ++y)
        {
            if (x != y)
            {
                std::cout << list_of_names[y] << " " << list_of_names[x];
                for (int i = 0; i < 40 - list_of_names[y].size() - list_of_names[x].size(); ++i)
                {
                    std::cout << " ";
                }
                std::cout << list_of_names[x] << " " << list_of_names[y] << std::endl;
            }
        }
        std::cout << "-------------------------------------------------------" << std::endl;
    }
    return 0;
}

// Sonic Bits
// Bytehoven
// Wave Warden
// Audio Admin
// Sonic Algebra
// Sonic Surgeon
// Silicon Sonic
// Sonic Science
// Sonic Supplies
// Wired Waves
// Audio Anvil
// Sonic Anvil
// Audio Armor
// Sonic Savage
// Sonic Sausage
// Sonic Snail
// 
