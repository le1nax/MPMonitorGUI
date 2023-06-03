/*#include <gui_application.hpp>

// This is our main application class. Inherit from gui_application.
class my_app : public gui_application
{
public:
    my_app()
        : value{ 0.1f }
        , show_demo{ false }
    { }

    // This method gets called every frame to render our gui.
    virtual void render() override
    {
        ImGui::Begin("Test");

        ImGui::Text("Value is %.2f.", value);
        ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);
        if (ImGui::Button("Reset")) // Returns true when pressed this frame
        {
            value = 0.55f;
            std::cout << "Value was set to: " << value << '\n';
        }

        ImGui::SameLine();
        ImGui::Checkbox("Show Demo", &show_demo);

        ImGui::End();

        if (show_demo) ImGui::ShowDemoWindow(&show_demo);
    }

private:
    float value;
    bool  show_demo;
};


// To run any class which has 'gui_application' as a public base, we can define ENTRY_POINT as the name of our class.
// If we include 'gui_main.hpp' afterwards this class will get executed. Do not add a main method if you use this header.
#define ENTRY_POINT my_app
#include <gui_main.hpp>
*/