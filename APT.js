
APT.On(
    "Import",
    function()
    {
        // AbytekAIAgent
        let AbytekAIAgentProject = APT.Extensions.ScopeLinker.Ensure({
            GitDetails: {
                RemoteURL: "git@github.com:Abytek/AIAgent.git"
            }
        });

        // AbytekSlang
        let AbytekSlangProject = APT.Extensions.ScopeLinker.Ensure({
            GitDetails: {
                RemoteURL: "git@github.com:Abytek/slang.git"
            }
        });
        APT.Set("AbytekSlang", AbytekSlangProject);
        APT.Push(
            "AbytekEBT.CMakeDefinitions", 
            [ "ABYTEK_SLANG_DIR", AbytekSlangProject.EntryDirectory ]
        );

        // Tracy
        let TracyProject = APT.Extensions.ScopeLinker.Ensure({
            GitDetails: {
                RemoteURL: "git@github.com:Abytek/tracy.git"
            }
        });
        APT.Set("Tracy", TracyProject);
        APT.Push(
            "AbytekEBT.CMakeDefinitions", 
            [ "ABYTEK_TRACY_DIR", TracyProject.EntryDirectory ]
        );

        // ImGui
        let ImGuiProject = APT.Extensions.ScopeLinker.Ensure({
            GitDetails: {
                RemoteURL: "git@github.com:Abytek/imgui.git"
            }
        });
        APT.Set("ImGui", ImGuiProject);
        APT.Push(
            "AbytekEBT.CMakeDefinitions", 
            [ "ABYTEK_IMGUI_DIR", ImGuiProject.EntryDirectory ]
        );
    }
);

let AbytekEBTSetupOptions = {
    IsEngine: true
};
APT.Extensions.AbytekEBT.SetupProject(this, AbytekEBTSetupOptions);