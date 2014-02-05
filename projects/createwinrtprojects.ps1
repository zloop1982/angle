. ".\scripts\createsln.ps1"
. ".\scripts\converterutils.ps1"
. ".\scripts\winrtconverter.ps1"

#$WorkingDir = (Get-Location).Path
$WorkingDir = $PSScriptRoot

$InputDir = join-path $WorkingDir ".\src" -Resolve
$WinRTDir = join-path $WorkingDir "src_winrt"
$WP8Dir = join-path $WorkingDir "src_wp8"
Write-Host "Converting projects..."

#create VS2013 winrt project files

Function ConvertAll($outdir, $platform)
{
    Get-ChildItem $InputDir  -Recurse -Include "*.vcxproj" | foreach-object {
        if($_.Name -ne "commit_id.vcxproj" -and $_.Name -ne "copy_compiler_dll.vcxproj")
        {
            $project = $_.Fullname
            $output = join-path $outdir (Get-RelativePath $InputDir  $project)

            try
            {
                Write-Host ("Creating: " + $output)
                WinrtConverter $_.Fullname $output $platform >> $null
            }
            catch
            {
                Write-Host ("Did not convert " + $project)        
                Write-Error ("Error: "  + $_)
            }
        }
    }
}

Function CreateSolutionFiles($dir)
{
    $preprocessor = join-path $dir "preprocessor.vcxproj" -Resolve
    $translator = join-path $dir "translator.vcxproj" -Resolve
    $translator_static = join-path $dir "translator_static.vcxproj" -Resolve

    $glesV2 = join-path $dir "libGLESv2.vcxproj" -Resolve
    AddProjectReference $glesV2 ($translator, $preprocessor) >> $null

    $egl = join-path $dir "libEGL.vcxproj" -Resolve
    AddProjectReference $egl $glesV2 >> $null

    $output = join-path $dir "angle.sln"
    CreateSolutionFile $output ($eglProject, $glesProject, $preprocessor, $translator,$translator_static)

}

#clone Win32 Project files to WinRT and WP8
ConvertAll $WinRTDir "winrt"
ConvertAll $WP8Dir "wp8"

#create .sln and project references
CreateSolutionFiles $WinRTDir
CreateSolutionFiles $WP8Dir



