pipeline {
  agent {
    node {
      label 'Windows'
      customWorkspace 'SteamBridge/Plugins/SteamBridge'
    }

  }
  stages {
    stage('Prep') {
      steps {
        bat 'C:\\build-scripts/SteamBridge/prep.bat'
      }
    }

    stage('Generate') {
      steps {
        bat 'C:\\build-scripts/SteamBridge/generate.bat'
      }
    }

    stage('Build') {
      steps {
        bat 'C:\\build-scripts/SteamBridge/build.bat'
      }
    }

  }
}
