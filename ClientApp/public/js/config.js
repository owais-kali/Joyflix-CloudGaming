export async function getServerConfig() {
  const protocolEndPoint = location.origin + '/config';
  const createResponse = await fetch(protocolEndPoint);
  return await { useWebSocket: true, startupMode: true, logging: true }
}

export function getRTCConfiguration() {
  let config = {};
  config.sdpSemantics = 'unified-plan';
  config.iceServers = [{ urls: ['stun:stun.l.google.com:19302'] }];
  return config;
}