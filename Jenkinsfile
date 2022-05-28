pipeline {
  options {
        disableConcurrentBuilds()
    }
  agent {
    node {
      label 'Windows'
    }
  }
  stages {
    stage('Build') {
      steps {
        bat "C:\\JenkinsBuilder.exe -p SteamBridge -c Build -w \"%WORKSPACE%\" -v 2022 -e \"C:\\\\UnrealEngine\\\\UE_5.0\\\\\""
      }
    }

    stage('Publish') {
      when {
        branch "main"
      }
      steps {
        bat "C:\\JenkinsBuilder.exe -p SteamBridge -c Publish -w \"%WORKSPACE%\""
      }
    }
  }
}
