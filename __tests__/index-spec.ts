import * as index from '../src/index';

test('Should have udp available', () => {
  expect(index.udp).toBeTruthy();
});
