
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "Demo");
        APT.Push(
            "AbytekEBT.Events.BeforePrepare",
            function()
            {
                APT.Import("../DemoPackage")
            }
        );
    }
);
APT.Extensions.AbytekEBT.SetupProject(this);