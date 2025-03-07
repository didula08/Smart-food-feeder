import { Image, StyleSheet, Switch } from 'react-native';
import ParallaxScrollView from '@/components/ParallaxScrollView';
import { ThemedText } from '@/components/ThemedText';
import { ThemedView } from '@/components/ThemedView';
import { useState } from 'react';
import Icon from 'react-native-vector-icons/MaterialIcons'; // Importing Material Icons

export default function HomeScreen() {
  const [isOn, setIsOn] = useState(true);
  const [newAction, setNewAction] = useState(false);

  return (
    <ParallaxScrollView
      headerBackgroundColor={{ light: '#4CAF50', dark: '#1D3D47' }}
      headerImage={
        <Image
          source={require('@/assets/images/petimage.jpg')}
          style={styles.headerImage}
        />
      }>
      
      <ThemedView style={[styles.container, { backgroundColor: '#f5f5f5' }]}>

        {/* Wi-Fi Status Card */}
        <ThemedView style={styles.statusBox}>
          <Icon name="wifi" size={30} color="#388E3C" />
          <ThemedText style={styles.statusText}>Wi-Fi: ON</ThemedText>
        </ThemedView>

        {/* Battery Status Card */}
        <ThemedView style={styles.statusBox}>
          <Icon name="battery-full" size={30} color="#FFEB3B" />
          <ThemedText style={styles.statusText}>Battery: 80%</ThemedText>
        </ThemedView>

        {/* Rounds Status Card */}
        <ThemedView style={styles.statusBox}>
          <Icon name="refresh" size={30} color="#3F51B5" />
          <ThemedText style={styles.statusText}>3 Rounds</ThemedText>
        </ThemedView>

        {/* Switches Card */}
        <ThemedView style={styles.switchCard}>
          <ThemedView style={styles.switchContainer}>
            {/* System Switch */}
            <ThemedText style={[styles.switchLabel, { color: isOn ? '#388E3C' : '#D32F2F' }]}>System</ThemedText>
            <Switch
              value={isOn}
              onValueChange={setIsOn}
              trackColor={{ false: "#D32F2F", true: "#388E3C" }}
              thumbColor="#fff"
              ios_backgroundColor="#D32F2F"
            />
          </ThemedView>

          <ThemedView style={styles.switchContainer}>
            {/* Motor Controller Switch */}
            <ThemedText style={[styles.switchLabel, { color: newAction ? '#388E3C' : '#D32F2F' }]}>Motor Controller</ThemedText>
            <Switch
              value={newAction}
              onValueChange={setNewAction}
              trackColor={{ false: "#D32F2F", true: "#388E3C" }}
              thumbColor="#fff"
              ios_backgroundColor="#D32F2F"
            />
          </ThemedView>
        </ThemedView>

      </ThemedView>
    </ParallaxScrollView>
  );
}

const styles = StyleSheet.create({
  headerImage: {
    width: '100%',
    height: '100%',
    resizeMode: 'cover',
  },
  container: {
    flex: 1,
    alignItems: 'center',
    paddingVertical: 30,
    paddingHorizontal: 20,
  },
  statusBox: {
    backgroundColor: '#fff',
    width: '90%', // Ensures uniform width for all cards
    padding: 20,
    marginVertical: 10,
    borderRadius: 15,
    elevation: 4,
    alignItems: 'center',
    justifyContent: 'center',
    flexDirection: 'row', // Align icon and text horizontally
  },
  statusText: {
    fontSize: 18,
    fontWeight: 'bold',
    color: '#333',
    marginLeft: 10, // Space between icon and text
  },
  switchCard: {
    backgroundColor: '#fff',
    width: '90%', // Ensures consistent width with other cards
    padding: 20,
    marginVertical: 10,
    borderRadius: 15,
    elevation: 6,
  },
  switchContainer: {
    flexDirection: 'row',
    backgroundColor: '#fff',
    alignItems: 'center',
    marginTop: 20,
    justifyContent: 'space-between',
  },
  switchLabel: {
    fontSize: 18,
    fontWeight: 'bold',
  },
});
