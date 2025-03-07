import React, { useState } from 'react';
import {
  StyleSheet,
  FlatList,
  TouchableOpacity,
  View,
  Alert,
} from 'react-native';
import ParallaxScrollView from '@/components/ParallaxScrollView';
import { ThemedText } from '@/components/ThemedText';
import { ThemedView } from '@/components/ThemedView';
import { IconSymbol } from '@/components/ui/IconSymbol';

// Define types for better type safety
type FeedingTime = {
  id: number;
  title: string;
  date: string;
  time: string;
};

export default function TabTwoScreen() {
  const [feedingTimes, setFeedingTimes] = useState<FeedingTime[]>([
    { id: 1, title: 'FEEDING TIME 1', date: '2021-09-06', time: '19:39' },
    { id: 2, title: 'FEEDING TIME 2', date: '2021-09-05', time: '13:50' },
    { id: 3, title: 'FEEDING TIME 3', date: '2021-09-05', time: '15:50' },
    { id: 4, title: 'FEEDING TIME 4', date: '2021-09-05', time: '10:50' },
  ]);

  const handleDelete = (id: number) => {
    Alert.alert(
      'Delete Feeding Time',
      'Are you sure you want to delete this feeding time?',
      [
        { text: 'Cancel', style: 'cancel' },
        {
          text: 'Delete',
          style: 'destructive',
          onPress: () => setFeedingTimes((prev) => prev.filter((item) => item.id !== id)),
        },
      ],
      { cancelable: true }
    );
  };

  const renderFeedingTimeItem = ({ item }: { item: FeedingTime }) => (
    <FeedingTimeItem item={item} onDelete={handleDelete} />
  );

  return (
    <ParallaxScrollView
      headerBackgroundColor={{ light: '#E3F2FD', dark: '#0D47A1' }}
      headerImage={
        <IconSymbol
          size={310}
          color="#64B5F6"
          name="chevron.left.forwardslash.chevron.right"
          style={styles.headerImage}
        />
      }
    >
      <ThemedView style={styles.titleContainer}>
        <ThemedText type="title" style={styles.titleText}>
          Feeding Times
        </ThemedText>
      </ThemedView>

      {/* Set New Feeding Time Section */}
      <ThemedView style={styles.sectionContainer}>
        <ThemedText type="subtitle" style={styles.sectionTitle}>
          Set New Feeding Time
        </ThemedText>
        <TouchableOpacity style={styles.setAlarmButton}>
          <ThemedText style={styles.setAlarmButtonText}>
            + Add Feeding Time
          </ThemedText>
        </TouchableOpacity>
      </ThemedView>

      {/* Scheduled Feeding Times Section */}
      <ThemedView style={styles.sectionContainer}>
        <ThemedText type="subtitle" style={styles.sectionTitle}>
          Scheduled Feeding Times
        </ThemedText>
        <FlatList
          data={feedingTimes}
          renderItem={renderFeedingTimeItem}
          keyExtractor={(item) => item.id.toString()}
          contentContainerStyle={styles.alarmList}
        />
      </ThemedView>
    </ParallaxScrollView>
  );
}

// Extracted FeedingTimeItem component for better reusability
const FeedingTimeItem = ({
  item,
  onDelete,
}: {
  item: FeedingTime;
  onDelete: (id: number) => void;
}) => (
  <View style={styles.alarmItem}>
    <View style={styles.alarmInfo}>
      <ThemedText type="defaultSemiBold" style={styles.alarmTitleText}>
        {item.title}
      </ThemedText>
      <ThemedText style={styles.alarmDateTime}>
        {item.date} • {item.time}
      </ThemedText>
    </View>
    <TouchableOpacity
      style={styles.deleteButton}
      onPress={() => onDelete(item.id)}
      accessibilityLabel="Delete feeding time"
    >
      <ThemedText style={styles.deleteButtonText}>Delete</ThemedText>
    </TouchableOpacity>
  </View>
);

const styles = StyleSheet.create({
  headerImage: {
    color: '#64B5F6',
    bottom: -90,
    left: -35,
    position: 'absolute',
  },
  titleContainer: {
    flexDirection: 'row',
    gap: 8,
    marginBottom: 20,
    padding: 15,
    backgroundColor: '#E3F2FD',
    borderRadius: 12,
    alignItems: 'center',
    justifyContent: 'center',
  },
  titleText: {
    color: '#0D47A1', // Deep Blue
    fontSize: 24,
    fontWeight: 'bold',
  },
  sectionContainer: {
    marginBottom: 20,
    padding: 15,
    backgroundColor: '#FFFFFF',
    borderRadius: 12,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 6,
    elevation: 3,
  },
  sectionTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    marginBottom: 10,
    color: '#FF5722', // Orange
  },
  alarmList: {
    flexGrow: 1,
  },
  alarmItem: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    padding: 12,
    backgroundColor: '#F5F5F5',
    borderRadius: 8,
    marginBottom: 10,
  },
  alarmInfo: {
    flex: 1,
  },
  alarmTitleText: {
    color: '#000', // Black
    fontSize: 16,
    fontWeight: 'bold',
  },
  alarmDateTime: {
    color: '#673AB7', // Purple
    fontSize: 14,
  },
  deleteButton: {
    backgroundColor: '#FF5252',
    paddingHorizontal: 12,
    paddingVertical: 6,
    borderRadius: 6,
  },
  deleteButtonText: {
    color: '#FFFFFF', // White
    fontWeight: 'bold',
  },
  setAlarmButton: {
    backgroundColor: '#4CAF50',
    padding: 15,
    borderRadius: 12,
    alignItems: 'center',
    justifyContent: 'center',
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.2,
    shadowRadius: 4,
    elevation: 3,
  },
  setAlarmButtonText: {
    color: '#FFFFFF', // White
    fontSize: 16,
    fontWeight: 'bold',
  },
});

