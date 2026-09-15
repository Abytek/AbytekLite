
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.RHISamples.UploadTexture");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_RHISampleModule_UploadTexture"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.RHISamples.UploadTexture.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);