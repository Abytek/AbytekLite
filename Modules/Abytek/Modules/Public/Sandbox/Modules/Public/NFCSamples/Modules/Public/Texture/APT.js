
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.NFCSamples.Texture");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_NFCSampleModule_Texture"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.NFCSamples.Texture.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);