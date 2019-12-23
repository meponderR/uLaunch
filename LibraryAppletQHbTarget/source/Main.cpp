#include <hb/hb_Target.hpp>
#include <am/am_QCommunications.hpp>

extern "C"
{
    u32 __nx_applet_type = AppletType_LibraryApplet;
    u32 __nx_applet_exit_mode = 2;

    void __appExit(void)
    {
    }
}

hb::TargetInput hb_input;
bool target_once = true; // Library applet targets once

int main()
{
    hb_input = {};

    hb_input.nro_path[0] = '\0';
    hb_input.argv[0] = '\0';

    // Initialize applet, read stuff, close applet
    appletInitialize();
    LibAppletArgs largs;
    auto rc = am::QLibraryAppletReadStorage(&largs, sizeof(largs));
    if(R_SUCCEEDED(rc))
    {
        if(largs.LaVersion == am::Magic)
        {
            hb::TargetInput ipt = {};
            rc = am::QLibraryAppletReadStorage(&ipt, sizeof(ipt));
            if(R_SUCCEEDED(rc))
            {
                if(strlen(ipt.nro_path))
                {
                    strcpy(hb_input.nro_path, ipt.nro_path);
                    if(strlen(ipt.argv)) strcpy(hb_input.argv, ipt.argv);
                }
            }
        }
    }
    appletExit();

    if(hb_input.nro_path[0] == '\0') strcpy(hb_input.nro_path, "sdmc:/hbmenu.nro");
    if(hb_input.argv[0] == '\0') strcpy(hb_input.argv, hb_input.nro_path);

    hb::Target(hb_input, target_once);

    return 0;
}