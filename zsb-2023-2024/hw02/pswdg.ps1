Set-ExecutionPolicy RemoteSigned -Force

Function UserExists {   # Check if a user you prompted exists
    param (
        [string]$username
    )

    try {
        $user = Get-LocalUser -Name $username -ErrorAction Stop
        return $true
    } catch {
        return $false
    }
}



Function check_password_policy {    # Check for password legitimacy
    param (
        [string]$password
    );

    $script:has_upper = $password -cmatch "[A-Z]";
    $script:has_lower = $password -cmatch "[a-z]";
    $script:has_number = $password -cmatch "[0-9]";
    $script:has_special = $password -cmatch '[^a-zA-Z0-9]';
    $script:long = $password.Length -ge 12;

    return $has_upper -and $has_lower -and $has_number -and $has_special -and $long;
}

do {    # Main cycle
    $username = Read-Host "Enter the username: ";

    if (UserExists -username $username) {
        Write-Host "User $username already exists. Please choose a different username.`n"
        continue
    }

    $password_secure = Read-Host "Enter the password: " -AsSecureString;

    $secure_string_conversion = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($password_secure);
    $p_pas = [System.Runtime.InteropServices.Marshal]::PtrToStringBSTR($secure_string_conversion);
    [System.Runtime.InteropServices.Marshal]::ZeroFreeBSTR($secure_string_conversion);

    $is_valid = check_password_policy -Password $p_pas;

    if (-not $is_valid) {
        Write-Host "Password doesn't meet the requirements.`n"; # This thing prints stuff you missed in your password
        if (-not $has_upper) {
            Write-Host "No uppercase letter used.";
        }
        if (-not $has_lower) {
            Write-Host "No lowercase letter used.";
        }
        if (-not $has_number) {
            Write-Host "No number used.";
        }
        if (-not $has_special) {
            Write-Host "No special character used.";
        }
        if (-not $long) {
            Write-Host "The password too short (12 characters min).";
        }

        Write-Host "Try again...`n";
    }

} while (-not $is_valid);

$computer_name = $env:COMPUTERNAME;
$account = "$computer_name/$username";
New-LocalUser -Name $username -Password $password_secure -Description "Created.";

Write-Host "User $account created.`n";
