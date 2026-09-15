
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.RHISamples.UploadBuffer");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_RHISampleModule_UploadBuffer"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.RHISamples.UploadBuffer.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);