
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Abytek.Sandbox.NFCSamples.StaticMeshImportFile");
        APT.Push(
            "AbytekEBT.ApplicationModuleClasses",
            "Abytek::F_NFCSampleModule_StaticMeshImportFile"
        );
        APT.Set(
            "AbytekEBT.IncludeHeaders",
            [
                "Abytek/Sandbox.NFCSamples.StaticMeshImportFile.hpp"
            ]
        );
    }
);

APT.Extensions.AbytekEBT.SetupApplication(this);