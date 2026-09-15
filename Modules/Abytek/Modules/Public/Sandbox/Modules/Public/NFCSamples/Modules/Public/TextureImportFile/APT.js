
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.NFCSamples.TextureImportFile");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_NFCSampleModule_TextureImportFile"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.NFCSamples.TextureImportFile.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);