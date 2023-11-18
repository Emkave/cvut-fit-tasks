Import-Module NetSecurity

function Add-FirewallRule {
    param (
        [string]$name,
        [string]$protocol,
        [int[]]$port,
        [string]$direction = "Inbound"
    )

    # Check if the rule already exists
    $rule = Get-NetFirewallRule -DisplayName $name -ErrorAction SilentlyContinue

    if (-not $rule) {
        # Create the firewall rule
        New-NetFirewallRule -DisplayName $name -Direction $direction -Action Allow -Protocol $protocol -LocalPort $port
        Write-Host "Rule $name created successfully."
    } else {
        Write-Host "Rule $name already exists."
    }
}

# Apply rules

# Web Server: Allow HTTP and HTTPS
Add-FirewallRule -name "Allow HTTP" -protocol "TCP" -port 80
Add-FirewallRule -name "Allow HTTPS" -protocol "TCP" -port 443

# Email Server: Allow SMTP, IMAP, and POP3
Add-FirewallRule -name "Allow SMTP" -protocol "TCP" -port 25
Add-FirewallRule -name "Allow IMAP" -protocol "TCP" -port @(143, 993)
Add-FirewallRule -name "Allow POP3" -protocol "TCP" -port @(110, 995)
